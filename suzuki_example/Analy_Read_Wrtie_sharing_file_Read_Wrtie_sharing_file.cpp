#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

class Sharing_file {
public:
	int32_t pl, ecc_id, fixedwall_id, trackerwall_id, spotid, zone[2], rawid[2], unix_time, tracker_track_id;
	//spotid:spotA * 100 + spotB
};

std::vector<Sharing_file> Read_sharing_file_bin(std::string filename);
std::vector<Sharing_file> Read_sharing_file_txt(std::string filename);
void Write_sharing_file_bin(std::string filename, std::vector<Sharing_file>&sharing_file_v);
void Write_sharing_file_txt(std::string filename, std::vector<Sharing_file>&sharing_file_v);

int main(int argc, char**argv) {
	if (argc != 4) {
		fprintf(stderr, "usage:prg in-filename out-filename mode\n");
		fprintf(stderr, "mode=0:read-binary write-binary\n");
		fprintf(stderr, "mode=1:read-binary write-txt\n");
		fprintf(stderr, "mode=2:read-txt write-binary\n");
		fprintf(stderr, "mode=3:read-txt write-txt\n");
		exit(1);
	}
	std::string file_in = argv[1];
	std::string file_out = argv[2];
	int mode = std::stoi(argv[3]);
	if (mode < 0 || mode>3) {
		fprintf(stderr, "mode exception\n");
		fprintf(stderr, "usage:prg in-filename out-filename mode\n");
		fprintf(stderr, "mode=0:read-binary write-binary\n");
		fprintf(stderr, "mode=1:read-binary write-txt\n");
		fprintf(stderr, "mode=2:read-txt write-binary\n");
		fprintf(stderr, "mode=3:read-txt write-txt\n");
		exit(1);
	}
	std::vector<Sharing_file> sharing_file_v;
	if (mode <= 1) sharing_file_v = Read_sharing_file_bin(file_in);
	else sharing_file_v = Read_sharing_file_txt(file_in);

	if (mode % 2 == 0) Write_sharing_file_bin(file_out, sharing_file_v);
	else Write_sharing_file_txt(file_out, sharing_file_v);

	return 0;
}
std::vector<Sharing_file> Read_sharing_file_bin(std::string filename) {
	std::vector<Sharing_file> ret;
	std::ifstream ifs(filename, std::ios::binary);
	//filesizeŽæ“¾
	ifs.seekg(0, std::ios::end);
	int64_t eofpos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	int64_t begpos = ifs.tellg();
	int64_t nowpos = ifs.tellg();
	int64_t size2 = eofpos - begpos;
	int64_t GB = size2 / (1000 * 1000 * 1000);
	int64_t MB = (size2 - GB * 1000 * 1000 * 1000) / (1000 * 1000);
	int64_t KB = (size2 - GB * 1000 * 1000 * 1000 - MB * 1000 * 1000) / (1000);
	if (GB > 0) {
		std::cout << "FILE size :" << GB << "." << MB << " [GB]" << std::endl;
	}
	else {
		std::cout << "FILE size :" << MB << "." << KB << " [MB]" << std::endl;
	}
	int64_t count = 0;
	Sharing_file t;
	while (ifs.read((char*)& t, sizeof(Sharing_file))) {
		if (count % 10000 == 0) {
			nowpos = ifs.tellg();
			auto size1 = nowpos - begpos;
			std::cerr << std::right << std::fixed << "\r now reading ..." << std::setw(4) << std::setprecision(1) << size1 * 100. / size2 << "%";
		}
		count++;

		ret.emplace_back(t);
	}
	auto size1 = eofpos - begpos;
	std::cerr << "\r now reading ..." << std::setw(4) << std::setprecision(1) << size1 * 100. / size2 << "%" << std::endl;;
	if (count == 0) {
		fprintf(stderr, "%s no data!\n", filename.c_str());
		exit(1);
	}
	return ret;
}
std::vector<Sharing_file> Read_sharing_file_txt(std::string filename) {
	std::vector<Sharing_file> ret;
	std::ifstream ifs(filename);
	//filesizeŽæ“¾
	ifs.seekg(0, std::ios::end);
	int64_t eofpos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	int64_t begpos = ifs.tellg();
	int64_t nowpos = ifs.tellg();
	int64_t size2 = eofpos - begpos;
	int64_t GB = size2 / (1000 * 1000 * 1000);
	int64_t MB = (size2 - GB * 1000 * 1000 * 1000) / (1000 * 1000);
	int64_t KB = (size2 - GB * 1000 * 1000 * 1000 - MB * 1000 * 1000) / (1000);
	if (GB > 0) {
		std::cout << "FILE size :" << GB << "." << MB << " [GB]" << std::endl;
	}
	else {
		std::cout << "FILE size :" << MB << "." << KB << " [MB]" << std::endl;
	}
	int64_t count = 0;
	Sharing_file t;
	//while (ifs.read((char*)& t, sizeof(Sharing_file))) {
	while (ifs>>t.pl>>t.ecc_id>>t.fixedwall_id>>t.trackerwall_id>>t.spotid>>t.zone[0]>>t.rawid[0]>>t.zone[1]>>t.rawid[1]>>t.unix_time>>t.tracker_track_id){
	if (count % 10000 == 0) {
			nowpos = ifs.tellg();
			auto size1 = nowpos - begpos;
			std::cerr << std::right << std::fixed << "\r now reading ..." << std::setw(4) << std::setprecision(1) << size1 * 100. / size2 << "%";
		}
		count++;

		ret.emplace_back(t);
	}
	auto size1 = eofpos - begpos;
	std::cerr << "\r now reading ..." << std::setw(4) << std::setprecision(1) << size1 * 100. / size2 << "%" << std::endl;;
	if (count == 0) {
		fprintf(stderr, "%s no data!\n", filename.c_str());
		exit(1);
	}
	return ret;
}
void Write_sharing_file_bin(std::string filename, std::vector<Sharing_file>&sharing_file_v) {
	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs) {
		//file open Ž¸”s
		fprintf(stderr, "File[%s] is not exist!!\n", filename.c_str());
		exit(1);
	}
	if (sharing_file_v.size() == 0) {
		fprintf(stderr, "target data ... null\n");
		fprintf(stderr, "File[%s] has no text\n", filename.c_str());
	}
	else {
		int64_t count = 0;
		int64_t max = sharing_file_v.size();

		for (int i = 0; i < max; i++) {
			if (count % 10000 == 0) {
				std::cerr << std::right << std::fixed << "\r now writing ..." << std::setw(4) << std::setprecision(1) << count * 100. / sharing_file_v.size() << "%%";
			}
			count++;
			ofs.write((char*)& sharing_file_v[i], sizeof(Sharing_file));
		}
		std::cerr << std::right << std::fixed << "\r now writing ..." << std::setw(4) << std::setprecision(1) << count * 100. / sharing_file_v.size() << "%%" << std::endl;
	}
}
void Write_sharing_file_txt(std::string filename, std::vector<Sharing_file>&sharing_file_v) {
	std::ofstream ofs(filename);
	if (!ofs) {
		//file open Ž¸”s
		fprintf(stderr, "File[%s] is not exist!!\n", filename.c_str());
		exit(1);
	}
	if (sharing_file_v.size() == 0) {
		fprintf(stderr, "target data ... null\n");
		fprintf(stderr, "File[%s] has no text\n", filename.c_str());
	}
	else {
		int64_t count = 0;
		int64_t max = sharing_file_v.size();

		for (int i = 0; i < max; i++) {
			if (count % 10000 == 0) {
				std::cerr << std::right << std::fixed << "\r now writing ..." << std::setw(4) << std::setprecision(1) << count * 100. / sharing_file_v.size() << "%%";
			}
			count++;
			ofs << std::right << std::fixed
				<< std::setw(3) << std::setprecision(0) << sharing_file_v[i].pl << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].ecc_id << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].fixedwall_id << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].trackerwall_id << " "
				<< std::setw(4) << std::setprecision(0) << sharing_file_v[i].spotid << " "
				<< std::setw(2) << std::setprecision(0) << sharing_file_v[i].zone[0] << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].rawid[0] << " "
				<< std::setw(2) << std::setprecision(0) << sharing_file_v[i].zone[1] << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].rawid[1] << " "
				<< std::setw(10) << std::setprecision(0) << sharing_file_v[i].unix_time << " "
				<< std::setw(3) << std::setprecision(0) << sharing_file_v[i].tracker_track_id << std::endl;

		}
		std::cerr << std::right << std::fixed << "\r now writing ..." << std::setw(4) << std::setprecision(1) << count * 100. / sharing_file_v.size() << "%%" << std::endl;
	}
}

