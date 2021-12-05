#pragma once

#include "Generic_mapper.hpp"
#include <fstream>
#include <unordered_map>

namespace travel{
	class Csv_station_parser : public travel ::Generic_mapper
	{
	public:
		/* Constructeur*/
		Csv_station_parser(const std::string& _filename1, const std::string& _filename2)
		{
			read_stations(_filename1);
			read_connections(_filename2);
		}

		/* Destructeur*/
		~Csv_station_parser(){}

		/* Getter*/
		std::vector<std::pair<uint64_t, uint64_t>> get_compute_travel(uint64_t _start, uint64_t _end);
		

	protected:
		/* Lecture des stations*/
		void read_stations(const std::string& _filename) override;

		/* Lecture des connections inter stations*/
		void read_connections(const std::string& _filename) override;

		/* Calcul du trajet*/
		std::vector<std::pair<uint64_t, uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override;
		std::vector<std::pair<uint64_t, uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override;
		
	};


}