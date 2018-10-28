#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <list>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <functional>
#include <numeric>

using namespace std;

//method 1: construct DAG and remove edge based on rules, search segment tree with length N
//method 2: recursively allocate shifts, apply rules each allocation
//time: 1 outperform 2         memory: 2 outperform 1

/*
day schedule: two weeks, schedule on day (1,...,14)
FT and PT:
|{ti}intersect{6,7,13,14}|<=2
FT:
|{tk,1≤tk≤7}| ≤ 4, |{tl,8≤ tl≤14}| ≤ 4

daily schedule <==> shift schedule, one shift work per day
cost of each pattern per day is calculated at one shift per day level
*/
int weekend_shift[] = {6,7,13,14};
vector<int> firstweek_shift(7);
vectorn<int> secondweek_shift(7);

//weekend preference
int weekendCheck(vector<int> *allocvec, int weekend_shift[])
{
    vector<int> interset(4);
    vector<int>::iterator iter = set_intersection(weekend_shift,weekend_shift+4,
                     allocvec->begin(),allocvec->end(),interset.begin());
    if(iter-interset.begin()<=2){
        vector<int>().swap(interset);
        return 1;
    }
    vector<int>().swap(interset);
    return 0;
}

//part time policy
int ptpolicyCheck(vector<int> *allocvec, vector<int> firstweek_shift, vector<int> secondweek_shift)
{
    vector<int> interset(7);
    vector<int>::iterator iter = set_intersection(firstweek_shift.begin(),firstweek_shift.end(),
                     allocvec->begin(),allocvec->end(),interset.begin());
    if(iter-interset.begin()<=4){
        interset.clear();
        iter = set_intersection(secondweek_shift.begin(),secondweek_shift.end(),
                     allocvec->begin(),allocvec->end(),interset.begin());
        if(iter-interset.begin()<=4){
            vector<int>().swap(interset);
            return 1;
        }
    }
    vector<int>().swap(interset);
    return 0;
}

int checkAvailb(int sta_ind, vector<int> *allocvec, int pfflag)
{
    allocvec->push_back(sta_ind);
    if(weekendCheck(allocvec, weekend_shift)){
        if (pfflag)
        {
            allocvec->pop_back();
            return 1;
        } else {
            if(ptpolicyCheck(allocvec, firstweek_shift, secondweek_shift)){
                allocvec->pop_back();
                return 1;
            }
        }
    }
    allocvec->pop_back();
    return 0;
}

int genPattern(list<vector<int>> *patlist, vector<int> *shiftind_lis, int shiftnum, int start_shiftind, int ptftflag)
{
    int totsched = 14;
    if(shiftind_lis->size() == shiftnum)
    {
        patlist->push_back(*(shiftind_lis));
        delete shiftind_lis;
        return 1;
    } else {
        int i = 0;
        int posaloc=0;
        for (i = start_shiftind; i <= totsched; i++)
        {
            if(checkAvailb(i,shiftind_lis,ptftflag)>0) {
                vector<int> *tmplis = new vector<int>(shiftind_lis->size());
                copy(shiftind_lis->begin(),shiftind_lis->end(),tmplis->begin());
                tmplis->push_back(i);
                int start_shiftind_new = i + 1;
                int allochk=genPattern(patlist, tmplis, shiftnum, start_shiftind_new,ptftflag);
                if(allochk){
                    posaloc = posaloc+1;
                }
            }
        }
        if(posaloc > 0)
        {
            return 1;
        } else {
            delete shiftind_lis;
        }
    }
    return 0;
}

int main()
{
    list<vector<int>> *genpat_store = new list<vector<int>>();
    vector<int> *indvec = new vector<int>();

    std::generate(firstweek_shift.begin(), firstweek_shift.end(), [n = 1] () mutable { return n++; });
    std::generate(secondweek_shift.begin(), secondweek_shift.end(), [n = 0] () mutable { n=n+1;return n+7; });

    //PT generation
    for (int j=1; j<=7; j++){
       genPattern(genpat_store, indvec,j,1,0);
       indvec->clear();
    }

    //FT generation
    genPattern(genpat_store, indvec,10,1,1);

    //list<vector<int>>::iterator it = (*genpat_store).begin();
    cout << genpat_store->size()<<endl;

        //for test
    list<vector<int>>::iterator expiter = (*genpat_store).end();
    advance(expiter,-5);
    vector<int>::iterator patiter;
    for(patiter=(*expiter).begin();patiter<(*expiter).end();patiter++){
        cout<<(*patiter)<<", ";
    }
    

   delete indvec;
   delete genpat_store;

    return 0;
}
