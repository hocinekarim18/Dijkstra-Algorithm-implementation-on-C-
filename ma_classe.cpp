#include "ma_classe.hpp"
#include <sstream>

#define INF 10000000;

void travel::Csv_station_parser::read_stations(const std::string& _filename)
{
    std::cout << "Parsing stations" << std::endl;
    std::string premiere_ligne;
    std::ifstream file(_filename);
    std::getline(file, premiere_ligne);

    std::string tmp;
    std::string key1;
    
    while (!file.eof())
    {
        travel::Station station;
        std::getline(file, station.name, ',');      // Récuperer le nom de la station
        std::getline(file, key1, ',');              // Récupérer le l'identifiant de la station
        std::getline(file, station.line_id, ',');    // Récupérer le num de ligne de metro  
        std::getline(file, station.address, ',');    // Récupérer l'adresse de la station
        std::getline(file, station.line_name);      // Récupérer l'adresse le nom de la ligne

        uint64_t key = 0;
        try {
            key =(uint64_t) std::stoi(key1);
        }
        catch (std::invalid_argument const& e) {
            std::cerr << "Bad input: std::invalid_argument thrown" << std::endl; 
        }
        catch (std::out_of_range const& e) {
            key = (uint64_t)std::stoll(key1);
        }
     
        this->stations_hashmap[key] = station;
    }
    file.close();
    std::cout << "Parsing ending" << std::endl;
}

void travel ::Csv_station_parser:: read_connections(const std::string& _filename)
{
    std::cout << "Parsing connections" << std::endl;

    /* Eliminer la première ligne*/
    std::string premiere_ligne;
    std::ifstream file(_filename);
    std::getline(file, premiere_ligne);
    std::string time_transfer;
    std::string start_id;
    std::string stop_id;
    std::string tmp;
    while (!file.eof())
    {
        std::getline(file, start_id, ',');
        std::getline(file, stop_id, ',');
        std::getline(file, time_transfer);
    
        uint64_t key1 = 0;
        uint64_t key2 = 0;
        uint64_t time = 0;
       
        try {
            key1 = (uint64_t)std::stoi(start_id);
            key2 = (uint64_t)std::stoi(stop_id);
            time = (uint64_t)std::stoi(time_transfer);  
        }
        catch (std::invalid_argument const& e) {
            std::cerr << "Bad input: std::invalid_argument thrown" << std::endl; 
        }
        catch (std::out_of_range const& e) {
            std::cerr << "Integer overflow: std::out_of_range thrown" << std::endl;
            key1 = (uint64_t)std::stoll(start_id);
            key2 = (uint64_t)std::stoll(stop_id);
            time = (uint64_t)std::stoll(time_transfer);
        }
        this->connections_hashmap[key1][key2] = time;
    }
    file.close();
    std::cout << "Parsing ending" << std::endl;

}

std::vector<std::pair<uint64_t, uint64_t>> travel :: Csv_station_parser :: compute_travel(uint64_t _start, uint64_t _end)
{
    std :: vector<std::pair<uint64_t, uint64_t>> chemin;
    // Oredered map pour les distances et les predecesseur
    std::unordered_map<uint64_t, travel::Station > stations = this->stations_hashmap;
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t>> connections = this->connections_hashmap;
    std::unordered_map<uint64_t, uint64_t > distance;
    std::unordered_map<uint64_t, uint64_t> predecesseur;

    std::unordered_map<uint64_t, travel::Station >Q;
    for (auto it = stations.begin(); it != stations.end(); ++it)
        Q[it->first] = it->second;

    // Initialisation des distances
    std::cout << "initialisation" << std::endl;
    for (auto it = stations.begin(); it != stations.end(); ++it)
    {
         distance[it->first] = INF;
    }
    distance[_start] = 0;
    std::cout << "Calcul des distance" << std::endl;
    
    uint64_t s1 = 1;
    
    while (Q.size() != 0)
    {
        uint64_t mini = INF;
        //std::cout << "Qsize = " <<Q.size() << std::endl;
        // Calcul du minimum
        for (auto it = Q.begin(); it != Q.end(); ++it)
        {
            if (distance[it->first] < mini)
            {
                mini = distance[it->first];
                s1 = it->first;
            }
        }
        Q.erase(s1);
        // Mise à jour des distances
        for (auto it = connections[s1].begin(); it != connections[s1].end(); ++it)
        {
            if (distance[it->first] > distance[s1] + connections[s1][it->first])
            {
                distance[it->first] = distance[s1] + connections[s1][it->first];
                predecesseur[it->first] = s1;  
            }
        }
    }
    std::cout << "Recherche meilleur chemin" << std::endl;

    // Recherche du chemin les plus cours
    uint64_t s = _end;
    while (s != _start)
    {
        std::pair < uint64_t, uint64_t> p;
        p.first = s;
        p.second = connections[predecesseur[s]][s];
        chemin.push_back(p);
        s = predecesseur[s];
    }
    std::pair < uint64_t, uint64_t> p;
    p.first = _start;
    p.second = connections[predecesseur[s]][s];
    chemin.push_back(p);
    return chemin;
}


std::vector<std::pair<uint64_t, uint64_t>> travel :: Csv_station_parser::get_compute_travel(uint64_t _start, uint64_t _end)
{
    return travel::Csv_station_parser::compute_and_display_travel(_start, _end);
}


std::vector<std::pair<uint64_t, uint64_t> > travel::Csv_station_parser::compute_and_display_travel(uint64_t _start, uint64_t _end)
{
    std::vector<std::pair<uint64_t, uint64_t>> chemin = travel::Csv_station_parser::compute_travel(_start, _end);

    std::cout << "\nBest way from " << this->stations_hashmap[_start].name << " to " << this->stations_hashmap[_end].name << std::endl;
    uint64_t temps = 0;
    for (auto it = chemin.rbegin(); it != chemin.rend(); ++it)
    {
        temps += it->second;
        std::cout << this->stations_hashmap[it->first] << "  (" << temps << " secs)" << std::endl;
    }
    /* Comptage du temps de trajet*/
    uint64_t min = 0;
    uint64_t sec = 0;
    min = temps / 60;
    sec = temps - min * 60;
    std::cout <<"\nTemps total pour arriver a destination: "<< min<< " minutes et " << sec<< " seconds."  << std::endl;

    return chemin; 
}
