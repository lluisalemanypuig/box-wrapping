#include "lsbutton.hpp"

LSButton::LSButton(QWidget *p) : QPushButton(p)
{

}

void LSButton::set_ref_organizer(BoxOrganizer *r) {
	box_org = r;
}

void LSButton::set_input_label(QLabel *l) {
	input_label = l;
}

void LSButton::load() {
	box_org->clear_boxes();

	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open BWP instance"),
		"/home/lluis/Documents/projects/box-wrapping/inputs/",
		tr("BWP instance (*.in);;All Files (*)")
	);

	string stdname = fileName.toStdString();

	ifstream fin;
	fin.open(stdname.c_str());
	if (not fin.is_open()) {
		cerr << "Error: could not open '" << fileName.toStdString() << "'" << endl;
		return;
	}

	int W;
	fin >> W;
	box_org->set_max_width(W);

	int n,w,h;
	while (fin >> n >> w >> h) {
		for (int i = 0; i < n; ++i) {
			box_org->add_box(w,h);
		}
	}
	fin.close();

	box_org->repaint();

	size_t i = stdname.length() - 1;
	while (i > 0 and stdname[i] != '/') {
		--i;
	}
	if (i == 0) {
		input_label->setText(QString::fromStdString( stdname ));
	}
	else {
		input_label->setText(QString::fromStdString( stdname.substr(i + 1, stdname.length() - i) ));
	}
}

void LSButton::save() {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Store BWP solution"),
		"/home/lluis/Documents/projects/box-wrapping/outputs/hand-made/",
		tr("BWP instance (*.out);;All Files (*)")
	);

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

