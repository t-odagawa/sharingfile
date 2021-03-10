// system includes
#include <fstream>
#include <ctime>

// boost includes
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

// B2 includes
#include <B2Reader.hh>
#include <B2SpillSummary.hh>
#include <B2BeamSummary.hh>

// Ninjasf includes
// #include <Sharing_file.hpp>
class Sharing_file {
public :
  int32_t pl, ecc_id, fixedwall_id, trackerwall_id, spotid, zone[2], rawid[2], unix_time, tracker_track_id;
};

namespace logging = boost::log;

int main (int argc, char *argv[]) {

  logging::core::get()->set_filter
    (
     logging::trivial::severity >= logging::trivial::info
     );

  BOOST_LOG_TRIVIAL(info) << "=====Start!=====";

  if (argc != 3) {
    BOOST_LOG_TRIVIAL(info) << "Usage : " << argv[0]
			    << " <input ninjasf file path> <output txt file path>";
    std::exit(1);
  }

  try {

#ifdef TEXT_MODE
    std::ifstream ifs(argv[1]);
#else
    std::ifstream ifs(argv[1], std::ios::binary);
#endif
    Sharing_file t;

    std::ofstream ofs(argv[2]);
#ifdef TEXT_MODE
    int32_t tmp;
    while (ifs >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> t.unix_time >> tmp) {
#else
    while (ifs.read((char*)& t, sizeof(Sharing_file))) {
#endif
      time_t unixtime = (time_t)t.unix_time;
      tm *tm_event = localtime(&unixtime);

      int year = tm_event->tm_year + 1900;
      int month = tm_event->tm_mon + 1;
      int day = tm_event->tm_mday;
      BOOST_LOG_TRIVIAL(debug) << year << "-" << month << "-" << day;

      std::string filedir = "/home/t2k/odagawa/data/hitconv/";
      std::string filename = filedir + "neutrino_b2physics_fullsetup_fullstat_timedifcut0_loose_ninja_";
      filename += std::to_string(year) + "_" + std::to_string(month) + "_" + std::to_string(day);
      filename += ".root";
      BOOST_LOG_TRIVIAL(debug) << "Filename : " << filename;

      B2Reader reader(filename);
      int entry = 0;
      while (reader.ReadNextSpill() > 0) {
	auto &spill_summary = reader.GetSpillSummary();
	int spill_unixtime = (int)(spill_summary.GetBeamSummary().GetTimestamp());
	if (spill_unixtime == unixtime) break;
	entry++;
      }

      ofs << std::right << std::fixed
	  << std::setw(5) << std::setprecision(0) << year << " "
	  << std::setw(3) << std::setprecision(0) << month << " "
	  << std::setw(3) << std::setprecision(0) << day << " "
	  << std::setw(6) << std::setprecision(0) << entry << std::endl;

    }


  } catch (const std::runtime_error &error) {
    BOOST_LOG_TRIVIAL(fatal) << "Runtime error : " << error.what();
    std::exit(1);
  } catch (const std::invalid_argument &error) {
    BOOST_LOG_TRIVIAL(fatal) << "Invalid argument error : " << error.what();
    std::exit(1);
  }

  BOOST_LOG_TRIVIAL(info) << "=====Finish!=====";
  std::exit(0);
}
