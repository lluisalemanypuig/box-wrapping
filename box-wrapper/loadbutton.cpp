#include "loadbutton.hpp"

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

void LoadButton::read_input(ifstream& fin) {
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

void LoadButton::read_output(ifstream& fin) {
	vector<coord> tls, dims;

	int L;
	fin >> L;

	int xtl,ytl, xbr,ybr;
	while (fin >> xtl >> ytl >> xbr >> ybr) {
		tls.push_back(coord(xtl,ytl));
		dims.push_back(coord(xbr - xtl + 1, ybr - ytl + 1));
	}

	// 01234
	// bwp_*
	int i = 4;
	while (i < instance_name.length() and instance_name[i] != '_') {
		++i;
	}
	int W = atoi(instance_name.substr(4, i - 1 - 3 + 1).c_str());

	box_org->set_max_width(W);
	box_org->set_max_length(L);
	box_org->set_boxes(tls, dims);
}

/// PUBLIC

LoadButton::LoadButton(QWidget *p) : QPushButton(p)
{
	box_org = nullptr;
	input_label = nullptr;
	save_button = nullptr;
	defaultdir = "none";
	filter = "none";
}

void LoadButton::load() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open BWP instance"),
		QString::fromStdString(defaultdir),
		QString::fromStdString(filter + ";;All Files (*)")
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
		box_org->clear_boxes();
		read_input(fin);
	}
	else if (file_ext == "out") {
		box_org->clear_boxes();
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
