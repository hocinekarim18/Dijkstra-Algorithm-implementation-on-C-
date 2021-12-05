#include "ma_classe.hpp"


int main()
{
    /* Parsing stations*/
    std::string f1 = "./data/s.csv";
    std::string f2 = "./data/c.csv";
    /* Stations and connectons parsing*/
    travel::Csv_station_parser station_parser(f1, f2);
    std::unordered_map<uint64_t, travel::Station> stations = station_parser.get_stations_hashmap();
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t> > connections = station_parser.get_connections_hashmap();

   /* Computing travel*/
    /***
    * Voila un exemple de l'execution de l'algorithme Dijkstra
    * Station de départ:  Gare du Nord (line 5)
    * Station d'arrivé: Station: Porte de Vincennes (line 1)
    *
    * Note: Le dédoublement de l'affichage de certaine station correpond juste à un changement de quais.
    ***/
    std::vector<std::pair<uint64_t, uint64_t>> v = station_parser.get_compute_travel(1958, 1751);
    return 0;
}
