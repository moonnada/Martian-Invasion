
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>

using namespace std;

const int MAX_MARTIANS = 100;

struct Martian {
    std::string fname_;
    std::string lname_;
    std::string SSN_;
    Martian(std::string fn, std::string ln, std::string ssn)
    : fname_(fn), lname_(ln), SSN_(ssn) { }
    // you may add other things to the structure
};

struct sortByFirstName {
    bool operator() (Martian x, Martian  y) {
        return x.fname_ < y.fname_;
    }
};

// non class declarations
void displayMenu();
Martian MartianInfo();
bool compByFirstName(Martian *first, Martian *second);
bool compByLastName(Martian *first, Martian *second);
bool compBySsn(Martian *first, Martian *second);
enum class Criteria { first, last, ssn};

class MartianDatabase {
public:
    // creates a new empty Martian Database initialized with the
    // contents of filename.
    MartianDatabase(std::istream& infile);
    // add item m to database, return true if successfully added.
    bool addMartian(const Martian &m);
    // delete item with the ssn in database, return true if successfully
    // deleted.
    bool deleteMartian(std::string ssn);
    // return true if the database does not contain any items, false
    // otherwise.
    bool isEmpty();
    // return true if the database is at its maximum storage, false
    // otherwise.
    bool isFull();
    // prints the list of Martians, sorted by last name, sorted
    // by first name and by MSSN.
    void printMartians(Criteria type);
    // deletes all the dynamically created memory
    ~MartianDatabase();
    void createSortByFirstName(Martian m);
    void createSortByLastName(Martian m);
    void createSortBySsn(Martian m);
    
private:
    Martian* m_unsorted[MAX_MARTIANS];
    Martian* m_fname[MAX_MARTIANS];
    Martian* m_lname[MAX_MARTIANS];
    Martian* m_ssn[MAX_MARTIANS];
};

void MartianDatabase::createSortByFirstName(Martian m){
    string fn, ln, ssn;
    int count = 0;  // tracks the actual number of data elements in array
    // first create a copy of the unsorted data
    for (int i = 0; i < MAX_MARTIANS; i++){
        if (this->m_unsorted[i] != nullptr){
            // if actual data exists ie not a null ptr increment the count
            this->m_fname[i] = new Martian(fn, ln, ssn);
            this->m_fname[i] = this->m_unsorted[i];
            count++;
        }
        else {
            // mark the the end of the range of allocated objects, so that you can determine the range of the array
            this->m_fname[i] = nullptr;
            break;
        }
    }
    // now we sort the array by first name, sort expects the range of the array
    sort(this->m_fname, this->m_fname + count, compByFirstName);
}

void MartianDatabase::createSortByLastName(Martian m){
    string fn, ln, ssn;
    int count = 0;  // tracks the actual number of data elements in array
    // first create a copy of the unsorted data
    for (int i = 0; i < MAX_MARTIANS; i++){
        if (this->m_unsorted[i] != nullptr){
            // if actual data exists ie not a null ptr increment the count
            this->m_lname[i] = new Martian(fn, ln, ssn);
            this->m_lname[i] = this->m_unsorted[i];
            count++;
        }
        else{
            // mark the the end of the range of allocated objects, so that you can determine the range of the array
            this->m_lname[i] = nullptr;
            break;
        }
    }
    // now we sort the array by last name, sort expects the range of the array
    sort(this->m_lname, this->m_lname + count, compByLastName);
}

void MartianDatabase::createSortBySsn(Martian m){
    string fn, ln, ssn;
    int count = 0;  // tracks the actual number of data elements in array
    // first create a copy of the unsorted data
    for (int i = 0; i < MAX_MARTIANS; i++){
        if (this->m_unsorted[i] != nullptr){
            // if actual data exists ie not a null ptr increment the count
            this->m_ssn[i] = new Martian(fn, ln, ssn);
            this->m_ssn[i] = this->m_unsorted[i];
            count++;
        }
        else {
            // mark the the end of the range of allocated objects, so that you can determine the range of the array
            this->m_ssn[i] = nullptr;
            break;
        }
    }
    // now sort the array by ssn, sort expects the range of the array
    sort(this->m_ssn, this->m_ssn + count, compBySsn);
}

MartianDatabase::MartianDatabase(std::istream& infile){
    string line;
    string fn, ln, ssn;
    int indexptr = 0;
    while(getline(infile, line)){
        stringstream s(line);
        s>>fn;
        s>>ln;
        s>>ssn;
        this->m_unsorted[indexptr]=new Martian(fn, ln, ssn);
        indexptr++;
    }
    // mark the the end of the range of allocated objects
    // so that you can determine the range of the array
    this->m_unsorted[indexptr] = nullptr;
}

bool MartianDatabase::addMartian(const Martian &m){
    int index = 0;
    while(index < MAX_MARTIANS){
        // if space unallocated then add
        if (this->m_unsorted[index] == nullptr){
            this->m_unsorted[index]=new Martian(m.fname_, m.lname_, m.SSN_);
            // mark the the end of the range of allocated objects
            // so that you can determine the range of the array
            this->m_unsorted[index + 1] = nullptr;
            return true;
        }
        else {
            index++;
        }
    }
    return false;
}


bool MartianDatabase::deleteMartian(string ssn){
    for(int i=0; i<MAX_MARTIANS; i++){
        if (this->m_unsorted[i]->SSN_ == ssn) {
            if (this->m_unsorted[i] + 1 == nullptr) {
                // if this is the last obj, simply delete it
                delete this->m_unsorted[i];
                // mark the the end of the range of allocated objects
                // so that you can determine the range of the array
                this->m_unsorted[i] = nullptr;
                return true;
            }
            else {
                int start = i;
                do
                {
                    this->m_unsorted[start] = this->m_unsorted[start + 1];
                    start++;
                } while (this->m_unsorted[start] != nullptr);
                this->m_unsorted[start] = nullptr;
                return true;
            }
        }
    }
    return false;
}



bool MartianDatabase::isEmpty(){
    int count = 0;
    for (int i=0; i<MAX_MARTIANS; i++){  //if unsorted one's array is not empty, then count is increment. This meaning is the array is not empty.
        if(this->m_unsorted[i]!=nullptr)
            count++;
    }
    if(count==0)
        return true;
    else
        return false;
}

bool MartianDatabase::isFull(){
    for (int i=0; i<MAX_MARTIANS; i++){
        if(this->m_unsorted[i]==nullptr)    //checks arrays are null or not
            return false;
    }
    return true;
}

void MartianDatabase::printMartians(Criteria type){
    if (type == Criteria::first){     //print first name
        //int j = 1;
        for(int i=0; i<MAX_MARTIANS; i++){
           // if(this->m_fname[i]!=nullptr){
             //   cout<<j;
                cout<<setw(10)<<this->m_fname[i]->fname_<<setw(10)<<this->m_fname[i]->lname_<<setw(11)<<this->m_fname[i]->SSN_<<endl;
               // j++;
            }
        
        }
    
    else if (type == Criteria::last){   //print last name
       // int j = 1;
        for (int i = 0; i<MAX_MARTIANS; i++) {
           // if (this->m_lname[i] != nullptr) {
             //   cout<<j;
                cout <<setw(10)<< this->m_lname[i]->fname_ <<setw(10)<< this->m_lname[i]->lname_ <<setw(11)<< this->m_lname[i]->SSN_ << endl;
               // j++;
            }
           // else {
             //   break;
            //}
        }
    //}
    else if (type == Criteria::ssn) {     //print ssn
     //   int j = 1;
        for (int i = 0; i<MAX_MARTIANS; i++) {
            
       //     if (this->m_ssn[i] != nullptr) {
         //       cout<<j;
                cout <<setw(10)<< this->m_ssn[i]->fname_ <<setw(10)<< this->m_ssn[i]->lname_ <<setw(11)<< this->m_ssn[i]->SSN_ << endl;
           //     j++;
            }
           // else {
             //   break;
            }
        }
    }
}

MartianDatabase::~MartianDatabase(){    //destructor
    int i = 0;
    while(m_unsorted[i] != nullptr && i < MAX_MARTIANS){
        delete m_unsorted[i];
        i++;
    }
}

bool compByFirstName(Martian *first, Martian *second){
    return first->fname_ < second->fname_;
}

bool compByLastName(Martian *first, Martian *second){
    return first->lname_ < second->lname_;
}
bool compBySsn(Martian *first, Martian *second){
    return first->SSN_ < second->SSN_;
}

int main(){
    string filename;
    cout<<"Please enter the input data file name : ";
    cin>>filename; //accept user input
    ifstream infile(filename);
    if(!infile.is_open())
        cout<<"File couldn't open. "<<endl;
    MartianDatabase objMartian(infile);
    if(objMartian.isEmpty()){
        cout<<"Empty File" <<endl;
        return 1;
    }
    int choice = 0;
    while(choice !=6){
        displayMenu();
        choice = -1;
        string fn, ln, ssn;
        Martian infoMartian(fn, ln, ssn);
        bool works = true;             ////////I ADD LINE #301 ~ #309 FOR CHECKING AN OUT OF CHOICE BOUNDARY.  /////////////
        do{                             //////// FOR EXAMPLE, IF I TYPE 8 FOR CHOICE, IT IS NOT AN INTEGER 1 TO 6./////////////
            if(works)
                works = false;
            else
                cout << "Please enter an integer 1 to 6> ";
            cin>>choice;
            
        }while(!((choice >=1) && (choice <=6)));
        switch (choice)
        {
            case 1:    //add a martian
            {
                infoMartian = MartianInfo();
                objMartian.addMartian(infoMartian);
                break;
            }
            case 2:    //deletes a martian
            {
                string ssn;
                cout << "Please enter a SSN: ";
                cin >> ssn;
                //objMartian.deleteMartian(ssn);
                bool checkDelete = objMartian.deleteMartian(ssn);
                if(!checkDelete)
                    cout<< "SSN not found" <<endl;
                
                break;
            }
            case 3:   // print by first name
            {
                cout<<"---- Sorted by First Name -----"<<endl;
                cout<<"  First Name Last Name      MSSN"<<endl;
                objMartian.createSortByFirstName(infoMartian);
                objMartian.printMartians(Criteria::first);
                cout<<endl<<endl;
                break;
            }
            case 4:     // print by last name
            {
                cout<<"---- Sorted by Last Name -----"<<endl;
                cout<<"  First Name Last Name      MSSN"<<endl;
                objMartian.createSortByLastName(infoMartian);
                objMartian.printMartians(Criteria::last);
                cout<<endl<<endl;
                break;
            }
            case 5:      //print by ssn
            {
                cout<<"---- Sorted by Mars SSN -----"<<endl;
                cout<<"  First Name Last Name      MSSN"<<endl;
                objMartian.createSortBySsn(infoMartian);
                objMartian.printMartians(Criteria::ssn);
                cout<<endl<<endl;
                break;
            }
            case 6:
                // exit
                return 0;
                
            default:
                break;
        }
    }
}




void displayMenu(){
    cout<<" Martian Database "<<endl<<endl;
    cout<<" 1. Add a martian"<<endl;
    cout<<" 2. Delete a martian"<<endl;
    cout<<" 3. Print by first name"<<endl;
    cout<<" 4. Print by last name"<<endl;
    cout<<" 5. Print by SSN"<<endl;
    cout<<" 6. Quit"<<endl;
    cout<<"-----------------------"<<endl;
    cout<<"> ";
}

Martian MartianInfo(){
    string fname, lname, ssn;
    cout << "Please enter a first name: ";
    cin >> fname;
    cout << "Please enter a last name: ";
    cin >> lname;
    cout << "Please enter a SSN: ";
    cin >> ssn;
    Martian m(fname, lname, ssn);
    return m;
}







