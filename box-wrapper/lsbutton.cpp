#include "lsbutton.hpp"

/// NON-CLASS

inline
void split_path_filename(const string& full_path, string& filename, string& extension) {

	// iterator over 'full_path'
	size_t i = full_path.size() - 1;

	// extract extension
	size_t ext_length = 0;
	while (i > 0 and full_path[i] != '.') {
		--i;
		++ext_length;
	}
	extension = full_path.substr(i + 1, ext_length);

	// extract filename
	--i;
	size_t filename_length = 0;
	while (i > 0 and full_path[i] != '/') {
		--i;
		++filename_length;
	}
	filename = full_path.substr(i + 1, filename_length);
}

/// PRIVATE

void LSButton::read_input(ifstream& fin) {
	int W;
	fin >> W;
	box_org->set_max_width(W);

	int n,w,h;
	while (fin >> n >> w >> h) {
		for (int i = 0; i < n; ++i) {
			box_org->add_box(w,h);
		}
	}
}

void LSButton::read_output(ifstream& fin) {

}

/// PUBLIC

LSButton::LSButton(QWidget *p) : QPushButton(p)
{
	box_org = nullptr;
	input_label = nullptr;
	save_button = nullptr;
}

void LSButton::set_instance_name(const string& iname) {
	instance_name = iname;
}

void LSButton::load() {
	box_org->clear_boxes();

	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open BWP instance"),
		"../inputs/material/",
		tr("BWP instance (*.in);;All Files (*)")
	);

	string stdname = fileName.toStdString();

	// read file, if it exists
	ifstream fin;
	fin.open(stdname.c_str());
	if (not fin.is_open()) {
		cerr << "Error: could not open '" << fileName.toStdString() << "'" << endl;
		return;
	}

	string file_ext;
	split_path_filename(stdname, instance_name, file_ext);

	if (file_ext == "in") {
		read_input(fin);
	}
	else if (file_ext == "out") {
		read_output(fin);
	}
	else {
		cerr << "Error: file '" << stdname << "' could not be recognised as input or output" << endl;
	}
	fin.close();
	// finish reading file

	// update widgets
	box_org->repaint();
	input_label->setText(QString::fromStdString(instance_name));
	save_button->set_instance_name(instance_name);
}

void LSButton::save() {

	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Store BWP solution"),
		QString::fromStdString("../outputs/hand-made/" + instance_name + ".out"),
		tr("BWP instance (*.out);;All Files (*)")
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

