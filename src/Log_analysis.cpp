#include <iostream> 
#include <fstream>
#include <string>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/date_time.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <ctime>


namespace bt = boost::posix_time;
//using namespace std;

const std::locale format = std::locale(std::locale::classic(),new bt::time_input_facet("%d/%b/%Y:%H:%M:%S"));

bool cmp_with_first(const std::pair<time_t, int>  &p1, const std::pair<time_t, int> &p2)
{//Function for the alphabetical order in time when ints are equal.		
	if( p1.second == p2.second) return p1.first < p2.first;
	return p1.second > p2.second;
};

template<class T>
bool cmp(const std::pair<T, int>  &p1, const std::pair<T, int> &p2)
{		
	return p1.second > p2.second;
};


void insert_into_map (std::map<std::string,int> &mymap, std::string &inserted)
{
	if (mymap.find(inserted) != mymap.end()) {
		mymap[inserted] += 1;
	}
	else {
		mymap.insert(std::make_pair(inserted, 1));
	}	
};

std::time_t pt_to_time_t(const bt::ptime& pt)
{
	bt::ptime timet_start(boost::gregorian::date(1970, 1, 1));
	bt::time_duration diff = pt - timet_start;
	return diff.ticks() / bt::time_duration::rep_type::ticks_per_second;
};

time_t seconds_from_epoch(const std::string& s)
{
	bt::ptime pt;
	std::istringstream is(s);
	is.imbue(format);
	is >> pt;
	return pt_to_time_t(pt);
};

template<class T>
void sort_and_print(const std::map<T,int>& Map, int MaxNumberToPrint, std::ofstream& outfile, const int PrintType)
{
	std::vector<std::pair<T, int> > pairs(Map.begin(), Map.end());
	sort(pairs.begin(), pairs.end(), cmp<T>);
	int size = static_cast<int>(pairs.size());
	switch (PrintType){
		case 1:			
			for (int i = 0;  i < std::min(MaxNumberToPrint,size); i++)
				outfile << pairs[i].first << std::endl;
			break;
		default:
			for (int i = 0;  i < std::min(MaxNumberToPrint,size); i++)
				outfile << pairs[i].first << "," <<  pairs[i].second << std::endl;
	}
};

inline void new_whole_hour(std::map<time_t,int>& hour,std::map<time_t,int>& Hours, int& Q)
{
		Hours.insert(std::make_pair((hour.begin()->first),Q));	
		Q -= hour.begin()->second ;
		hour.erase(hour.begin());						
};

int main()
{
	typedef std::vector<std::string> StrSplit;
	std::ifstream log_file ("./log_input/log.txt");// File with logs
	std::ofstream hosts ("./log_output/hosts.txt");
	std::ofstream resources ("./log_output/resources.txt");
	std::ofstream hours ("./log_output/hours.txt");
	std::ofstream blocked ("./log_output/blocked.txt");
	std::ofstream notfound ("./log_output/not_found.txt");
	std::ofstream popres ("./log_output/pop_resources.txt");
	std::ofstream incorrect ("./log_output/incorrect.txt");
	std::ofstream codestat ("./log_output/code_statistics.txt");
	std::ofstream servererror ("./log_output/server_error.txt");

	if (!log_file.is_open()) return 1;
	if (!hosts.is_open()) return 2;
	if (!resources.is_open()) return 3;
	if (!hours.is_open()) return 4;
	if (!blocked.is_open()) return 5;
	if (!notfound.is_open()) return 6;
	if (!popres.is_open()) return 7;
	if (!incorrect.is_open()) return 8; 
	if (!codestat.is_open()) return 9; 
	if (!servererror.is_open()) return 10;

	std::map <std::string, int> MostActAcc;	 // List of active host/IP addresses that have accessed the site
	std::map <std::string, int> MostBand; 	 //List of resources that consume the most bandwidth on the site
	std::map <std::string, int> Sizes; 	 // List of sizes for website resorses
	std::map <std::string, int> NotFound;    // List of all not found pages (code 404)
	std::map <std::string, int> Answers;	 // List of all respond codes
	std::map <time_t, int> HourTimeLoop; 	 //Map for current 60 minutes period
	std::map <time_t, int> HoursBusyness; 	 //List of all 60 minutes periods
	std::map <std::string, time_t> Blocked;  // List of current Blocked host/IP addresses. We renew it each non-empty second.
	std::map <time_t, std::string> Users401; // List of requests with code 401. We renew it each time we have a 401 request
	
	std::string Line; 			 // String for reading logs 
	time_t CurSecond,d,Dif; 

	//std::map <time_t, std::string>::iterator iter;
	std::map <std::string, time_t>::iterator iterat;	
	//std::map <std::string, int>::iterator iterator;
	std::map <std::time_t, int>::iterator ite;
	
	StrSplit LineParts;
	int ReqCode,LSize;
	int QInHour=0;
	int SecondCount=0;
	while(getline(log_file, Line)){
		boost::split(LineParts, Line, boost::is_any_of(" "),boost::token_compress_on);   // Split line from "log.txt" into pieces	
		LSize=LineParts.size();
		if(LSize >=8) {
			insert_into_map(MostActAcc,LineParts[0]);
			insert_into_map(Answers,LineParts[LSize-2]);
			// Convert time of current request into seconds from epoch and add to HourTimeLoop
			LineParts[3].erase(LineParts[3].find('['), 1);
			CurSecond=seconds_from_epoch(LineParts[3]);
			if(HourTimeLoop.find(CurSecond) != HourTimeLoop.end()) {
				HourTimeLoop[CurSecond] += 1;
			}
			else if((HourTimeLoop.end()->first) < 1){
				HourTimeLoop.insert(std::make_pair(CurSecond, 1));
				SecondCount +=1;
			}
			else{  // Insert empty seconds into Hour time loop
				if((Dif=CurSecond-(HourTimeLoop.rbegin()->first))>1 ){
					for (d=Dif-1;d>0;d--){
						HourTimeLoop.insert(std::make_pair(CurSecond-d,0));
						SecondCount += 1;
						if (SecondCount==3600){
							new_whole_hour(HourTimeLoop, HoursBusyness, QInHour);
							SecondCount -= 1;
						}
					}
				}
				HourTimeLoop.insert(std::make_pair(CurSecond, 1));
				SecondCount +=1;
				if (SecondCount==3600){
					new_whole_hour(HourTimeLoop, HoursBusyness, QInHour);
					SecondCount -= 1;
				}
				// CurSecond is a garanteedly new second. We need to clean Blocked list 
				if((Blocked.end()->second)>0)
					for(iterat=Blocked.begin();iterat != Blocked.end(); iterat++)
						if((iterat->second) <= (CurSecond-300)) Blocked.erase(iterat--);
			}		
			QInHour += 1;//We anaway inserted one new request into HourTimeLoop
			if((iterat=Blocked.find(LineParts[0]))!=Blocked.end())
				blocked << Line << std::endl; //Write requests when host/IP is blocked
			ReqCode=atoi(LineParts[LSize-2].c_str());// Server code as an integer
			if((ReqCode == 401 )and(iterat==Blocked.end())){
				Users401.insert(make_pair(CurSecond,LineParts[0]));
				Users401.erase(Users401.begin(), Users401.lower_bound(CurSecond-20));
				int CurUser401Q = 0;
				for(auto iter = Users401.begin();iter != Users401.end(); iter++)
					if(iter->second == LineParts[0]) CurUser401Q +=1;
				if(CurUser401Q >= 3) 
					Blocked.insert(make_pair(LineParts[0],CurSecond));
			}
			else if(ReqCode == 404 )
				insert_into_map(NotFound,LineParts[6]);
			else if((ReqCode >= 500))
				servererror << Line << std::endl;
			insert_into_map(MostBand,LineParts[6]);
			if(Sizes.find(LineParts[6]) == Sizes.end())
				Sizes.insert(std::make_pair(LineParts[6],atoi(LineParts[LSize-1].c_str())));
		}
		else incorrect << Line << std::endl; //Write all incorrect requests
	} //Processing the last ten possible Most Busiest hours.
	if((QInHour) > 0){ 
		time_t FirstSecond = HourTimeLoop.begin()->first;
		HoursBusyness.insert(std::make_pair((FirstSecond),QInHour));
		QInHour -= HourTimeLoop.begin()->second;
		for (time_t r=1; r<10; r++){
			HoursBusyness.insert(std::make_pair((FirstSecond+r),QInHour));
			if((ite = HourTimeLoop.find(FirstSecond+r))!=HourTimeLoop.end())
				QInHour -= ite -> second;
		}
	}
	// Sort and print the most popular website parts  
	std::vector<std::pair<std::string, int> > pairs2(MostBand.begin(), MostBand.end());
	int i = 0;
	for(auto iterator=Sizes.begin(); iterator != Sizes.end(); iterator++){
		(iterator->second) = (iterator->second)*pairs2[i].second;	
		i += 1;			
	}
	// Sort Most_act_acc map 
	sort_and_print(MostActAcc, 10, hosts, 2);
	// Sort and print the most bandwidth-consuming resources
	sort_and_print(MostBand,50,popres,2);
	sort_and_print(Sizes,10,resources,1);
	// Sort and print in file notfound requests
	sort_and_print(NotFound,20,notfound,2);
	//HTTP1.0 respond codes statistics. Codes are in alphabetical order 
	int size = static_cast<int>(Answers.size());
	for (auto iterator=Answers.begin(); iterator!= Answers.end();iterator++) {
		codestat << iterator->first << "," << iterator -> second << std::endl;
	}

	// Sort Hours Busyness map 
	std::vector<std::pair<time_t, int> > pairs3(HoursBusyness.begin(), HoursBusyness.end());
	//pairs3=std::reverse(HoursBusyness.begin(), HoursBusyness.end());
	sort(pairs3.begin(), pairs3.end(), cmp<time_t>);
	pairs3.resize(10);
	sort(pairs3.begin(), pairs3.end(), cmp_with_first);
	size = static_cast<int>(pairs3.size());
	LineParts[4].erase(LineParts[4].find(']'), 1);
	//Convert epoch into human readable time and print results in file
	struct tm *ts;
	char buf[80];
	for (int i = 0; i < std::min(10,size); i++) {
		ts = gmtime(&pairs3[i].first);
		strftime(buf,sizeof(buf),"%d/%b/%Y:%H:%M:%S",ts);
		hours << buf <<" "<< LineParts[4]<< "," << pairs3[i].second << std::endl;
	}

	log_file.close();
	hosts.close();
	resources.close();
	hours.close();
	blocked.close();
	notfound.close();
	popres.close();
	incorrect.close(); 
	codestat.close();
	servererror.close();
	return 0;
}
