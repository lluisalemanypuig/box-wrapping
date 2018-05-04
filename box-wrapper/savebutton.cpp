#include "savebutton.hpp"

/// PUBLIC

SaveButton::SaveButton(QWidget *p) : QPushButton(p)
{
	box_org = nullptr;
}

void SaveButton::set_instance_name(const string& iname) {
	instance_name = iname;
}

void SaveButton::save() {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Store BWP solution"),
		QString::fromStdString("../outputs/hand-made/" + instance_name + ".out"),
		QString::fromStdString("BWP instance (*.out);;All Files (*)")
	);

	if (fileName == "") {
		return;
	}

	ofstream fout;
	fout.open(fileName.toStdString().c_str());
	if (not fout.is_open()) {
		cerr << "Error: could not open '" << fileName.toStdString() << "'" << endl;
		return;
	}

	int L;
	vector<coord> tls, brs;
	box_org->get_box_corners(L, tls, brs);

	fout << L << endl;
	for (size_t i = 0; i < tls.size(); ++i) {
		fout << tls[i].first << " " << tls[i].second;
		fout << "\t\t";
		fout << brs[i].first - 1 << " " << brs[i].second - 1;
		fout << endl;
	}

	fout.close();
}

