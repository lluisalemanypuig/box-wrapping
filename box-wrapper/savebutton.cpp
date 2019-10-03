/*********************************************************************
 * Box Wrapping solver
 * Copyright (C) 2018  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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

