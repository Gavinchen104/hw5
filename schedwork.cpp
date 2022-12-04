#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool solverHelper(const size_t n, const size_t k, const size_t dailyNeed, size_t row, size_t col, map<Worker_T, size_t> work, DailySchedule& sched, const size_t maxShifts, const AvailabilityMatrix& avail);
bool isValid(size_t row, size_t col, const size_t n, const size_t k, const size_t maxShifts, const AvailabilityMatrix& avail, map<Worker_T, size_t> work, const DailySchedule& sched, const size_t dailyNeed);
// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    size_t n = avail.size();
    size_t k = avail[0].size();
		//initialize a 2d array
		for(unsigned int i=0; i<n; i++){
			vector<Worker_T> vec(dailyNeed, k);
			sched.push_back(vec);
		}
		//initialize a map for the shift of each worker
		map<Worker_T, size_t> work;
		for(Worker_T c=0; c<k; c++){
			work[c]=0;
		}
		return solverHelper(n,k,dailyNeed,0,0, work, sched, maxShifts, avail);
}

bool solverHelper(const size_t n, const size_t k, const size_t dailyNeed, size_t row, size_t col, map<Worker_T, size_t> work, DailySchedule& sched, const size_t maxShifts, const AvailabilityMatrix& avail){
    // base case
    if(row == n) return true;
		for(Worker_T m = 0; m < k; m++){
			unsigned int count = 0;
			for(Worker_T i = 0; i < n; i++){
				if(avail[i][m] == 0){
					count++;
				}
			}
			if(count == n) return false;
		}

    // if not modified
    if(sched[row][col]==k){
			for(Worker_T i=0; i<k; i++){
				sched[row][col]=i;
				// if the worker id is valid
				if(isValid(row, i, n, k, maxShifts, avail, work,sched, dailyNeed)){

					// if the end of a row
					if(col==dailyNeed){
						if(solverHelper(n,k, dailyNeed, row+1, 0, work, sched, maxShifts, avail)==true) return true;
					}

					// if not the end of a row
					else{
						if(solverHelper(n, k, dailyNeed,row, col+1, work, sched, maxShifts, avail)==true)return true;
					}
				}
			}
			// backtracking 
			sched[row][col]=k;
		}
 
		else{
			// if the end of a row
			if(col==dailyNeed){
				if(solverHelper(n, k, dailyNeed, row+1, 0, work, sched, maxShifts, avail)==true) return true;
			}
			// if not the end of a row
			else{
				if(solverHelper(n, k, dailyNeed,row, col+1, work, sched, maxShifts, avail)==true) return true;
			}
		}

    return false;
}

bool isValid(size_t row, size_t col, const size_t n, const size_t k, const size_t maxShifts, const AvailabilityMatrix& avail, map<Worker_T, size_t> work, const DailySchedule& sched, const size_t dailyNeed){
  
	// check if worker is available 
  if(avail[row][col]== 0) return false;


	// check if a row has same value

  vector<Worker_T> temp = sched[row];
  map<Worker_T,size_t> temp_map;
  map<Worker_T,size_t>::iterator it;
  for(size_t i = 0; i < temp.size(); i++){
		it = temp_map.find(temp[i]);
		//if no repeated
		if(it == temp_map.end()){
			temp_map[temp[i]]=1;
		}

		//if repeated 
		else{
			temp_map[temp[i]]++;
			if(temp[i]!=k) return false;
		}
  }


  // check if shift bigger than maxShifts

	//counting the total number shift of a person
  for(size_t i = 0; i < n; i++){
		for(size_t m = 0; m < dailyNeed; m++){
			work[sched[i][m]]++;
		}
  }
  //if the work shift exceeds maxShifts return false
  for(it = work.begin(); it != work.end(); ++it){
		if(it->second > maxShifts && it->first != k) return false;
	}

	return true;
}