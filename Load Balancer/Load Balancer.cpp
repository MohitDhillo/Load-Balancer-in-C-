#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
#define rep(i,x,y) for(ll i=x;i<y;i++)
#define rrep(i,x,y) for(int i=x;i>=y;i--)
const ll MOD=1e18;
const ll MAXX=1e6+1; 

 int num;

class HttpRequest;

class Website{
    public:
    int website_id;
    int owner_id;
    int bandwidth;
    int processing_power;
    std::queue<pair<int,int>> queue;
    int total_weight;

    Website(int id, int owner, int bw, int power)
        : website_id(id), owner_id(owner), bandwidth(bw), processing_power(power), total_weight(power+bw) {
    }

    void add(int request_id,int processing_time){
        auto p=make_pair(request_id,processing_time);
      queue.push(p);
    }

};

vector<Website *> v;


class HttpRequest{

    public:
int request_id;
int website_id;
int processing_time;

HttpRequest(int x,int y,int z){
    request_id=x;
    website_id=y;
    processing_time=z;
}

void add(){
    for(int i=0;i<v.size();i++){
        if(v[i]->website_id==this->website_id){
            v[i]->add(request_id,processing_time);
            break;
        }
    }
}
        
};

class LoadBalancer{
    public:
      void add_website(int website_id,int owner_id,int bandwidth,int processing_power){
         Website* w=new Website(website_id,owner_id,bandwidth,processing_power);
        v.push_back(w);
        num=v.size();
      }

      void enqueue_request(HttpRequest r){
            r.add();
      }
      void dequeue_request(int id){
       for(int i=0;i<num;i++){
        if(v[i]->website_id==id){
            v[i]->queue.pop();
            break;
        }
       }
      }
      void WFQ_Scheduling(){
     
       map<int,double> weights;


           for(int i=0;i<num;i++){
               weights[i]=(v[i]->total_weight);

           }
           int cnt=0;
           int val;
           double tot=0;
           for(auto &it:weights){
            tot+=it.second;
           }
           for(auto &it:weights){

            it.second=double(it.second)/(tot);
            }
           
    //        //cout<<cnt<<endl;
    
           // Calculating Virtual Finish times
           multiset<pair<double,pair<pair<ll,ll>,ll>>> s;
           for(int i=0;i<num;i++){
            double time=0;
            while(!v[i]->queue.empty()){
               auto curr=v[i]->queue.front();
               v[i]->queue.pop();
               time+=double(curr.second)/weights[i];
               s.insert({time,{{curr.first,v[i]->website_id},curr.second}});
            }
           }
           vector<pair<pair<ll,ll>,ll>> ord;
           while(!s.empty()){
            auto curr=*(s.begin());
            s.erase(s.begin());
            ord.push_back(curr.second);
           }
           ll actual_time=0;
           for(auto it:ord){
            actual_time+=it.second;
            dequeue_request(it.first.second);
            cout<<"Request no "<<it.first.first<<" with required website "<<it.first.second<<" dequeued at "<<actual_time<<endl;
           }
      }
      };



LoadBalancer l;


void StressTesting(int numWebsites=2, int TotalRequests=10) {
    srand(43);
    LoadBalancer *WFQ = new LoadBalancer();
    cout << "Simulating " << numWebsites << " Websites with "<<TotalRequests<<" total requests" << endl;

    for (int i = 1; i <= numWebsites; ++i) {
        int b = (rand() % 200) + 1;  // Random bandwidth value between 1 and 200
        int p = (rand() % 100) + 1;  // Random processing power value between 1 and 100
        cout << "Website " << i << ": (Bandwidth, Processing Power) " << b << " " << p << endl;
        WFQ->add_website(i, i, b, p);
    }

    for (int i = 0; i < TotalRequests; ++i) {
        //WFQ->time++;
        int webid = (rand() % numWebsites) + 1;
        //int randomInt = (rand() % 100) + 1;
        int randomInt = 25;
        HttpRequest req(i + 1, webid, randomInt);
        cout<<"Request with id: "<<i+1<<" for website "<<webid<<" with a processing time of "<<randomInt<<endl;
        WFQ->enqueue_request(req);
    }

    WFQ->WFQ_Scheduling();
v.clear();
    delete WFQ;
}
void DiffBandwidths(int numWebsites=2, int TotalRequests=10) {
    srand(43);
    LoadBalancer *WFQ = new LoadBalancer();
    cout << "Simulating " << numWebsites << " Websites with "<<TotalRequests<<" total requests" << endl;

    for (int i = 1; i <= numWebsites; ++i) {
        int b = (rand() % 200) + 1;  // Random bandwidth value between 1 and 200
        int p = 12;  // Random processing power value between 1 and 100
        cout << "Website " << i << ": (Bandwidth, Processing Power) " << b << " " << p << endl;
        WFQ->add_website(i, i, b, p);
    }

    for (int i = 0; i < TotalRequests; ++i) {
        //WFQ->time++;
        int webid = (rand() % numWebsites) + 1;
        //int randomInt = (rand() % 100) + 1;
        int randomInt = 25;
        HttpRequest req(i + 1, webid, randomInt);
         cout<<"Request with id: "<<i+1<<" for website "<<webid<<" with a processing time of "<<randomInt<<endl;
        WFQ->enqueue_request(req);
    }
WFQ->WFQ_Scheduling();

    v.clear();
delete WFQ;
}

void DiffProcPower(int numWebsites=2, int TotalRequests=10) {
    srand(43);
    LoadBalancer *WFQ = new LoadBalancer();
    cout << "Simulating " << numWebsites << " Websites with "<<TotalRequests<<" total requests" << endl;

    for (int i = 1; i <= numWebsites; ++i) {
        int b = 20;  // Random bandwidth value between 1 and 200
        int p = (rand() % 200) + 1;  // Random processing power value between 1 and 100
        cout << "Website " << i << ": (Bandwidth, Processing Power) " << b << " " << p << endl;
        WFQ->add_website(i, i, b, p);
    }

    for (int i = 0; i < TotalRequests; ++i) {
       // WFQ->time++;
        int webid = (rand() % numWebsites) + 1;
        //int randomInt = (rand() % 100) + 1;
        int randomInt = 25;
        HttpRequest req(i + 1, webid, randomInt);
         cout<<"Request with id: "<<i+1<<" for website "<<webid<<" with a processing time of "<<randomInt<<endl;
        WFQ->enqueue_request(req);
    }

    WFQ->WFQ_Scheduling();
     v.clear();
    delete WFQ;
}


void EmptyQueues(int numWebsites, int TotalRequests=0)
{
    srand(43);
    LoadBalancer *WFQ = new LoadBalancer();
    cout << "Simulating " << numWebsites << " Websites with "<<TotalRequests<<" total requests" << endl;

    for (int i = 1; i <= numWebsites; ++i) {
        int b = (rand() % 200) + 1;  // Random bandwidth value between 1 and 200
        int p = (rand() % 100) + 1;  // Random processing power value between 1 and 100
        cout << "Website " << i << ": (Bandwidth, Processing Power) " << b << " " << p << endl;
        
        WFQ->add_website(i, i, b, p);
    }

   WFQ->WFQ_Scheduling();
    v.clear();
    delete WFQ;
}

void Equality(int numWebsites=2, int TotalRequests=10) {
    srand(43);
    LoadBalancer *WFQ = new LoadBalancer();
    cout << "Simulating " << numWebsites << " Websites with "<<TotalRequests<<" total requests" << endl;
    int b = (rand() % 200) + 1;  // Random bandwidth value between 1 and 200
    for (int i = 1; i <= numWebsites; ++i) {

        cout << "Website " << i << ": (Bandwidth, Processing Power) " << b << " " << b << endl;
        WFQ->add_website(i, i, b, b);
    }

    for (int i = 0; i < TotalRequests; ++i) {
        int webid = (rand() % numWebsites) + 1;
        int randomInt = 25;
        HttpRequest req(i + 1, webid, randomInt);
         cout<<"Request with id: "<<i+1<<" for website "<<webid<<" with a processing time of "<<randomInt<<endl;
        WFQ->enqueue_request(req);
    }

   WFQ->WFQ_Scheduling();
   v.clear();
    delete WFQ;
}

void manual_input(){
   cout<<"Enter the number of Websites: \n";
   int number;
    cin>>number;
  
   cout<<"Enter the websites in the order :- website_id, owner_id, bandwidth, processing power \n";
    for(int i=0;i<number;i++){
        int web_id;
        cin>>web_id;
        int owner_id;
        cin>>owner_id;
        int bandwidth;
        cin>>bandwidth;
        int processing_power;
        cin>>processing_power;
       l.add_website(web_id,owner_id,bandwidth,processing_power);
    }
   cout<<"Starting the WFQ alogirthm...\n";
cout<<"Enter the number of HTTP requests : \n";
 int sz;
 cin>>sz;
  cout<<"Enter input HTTP requests(request_id, website_id, processing_time) :\n";
 for(int i=0;i<sz;i++){
    int x,y,z;
    cin>>x>>y>>z;
    HttpRequest r(x,y,z);
    l.enqueue_request(r);
 }
 l.WFQ_Scheduling();
cout<<"Exiting...\n";
v.clear();


 
}



int main()
{

    int choice;
    cout<<"Enter the Test Case Number (1-8) or press 9 for manual input and any other key to exit: \n";
    int n,r;
    while(1)
    {
        cout<<"\nChoice: ";
        cin>>choice;
        switch(choice)
        {
            case 1:
                Equality();
                break;

            case 2:
                cout<<"Enter the Number of Websites: ";
                cin>>n;
                DiffBandwidths(n);
                break;
            
            case 3:
                cout<<"Enter the Number of Websites: ";
                cin>>n;
                DiffProcPower(n);
                break;
            
            case 4:
                cout<<"Enter the Number of Websites: ";
                cin>>n;
                Equality(n);
                break;

            case 5:
                cout<<"Enter the Number of Websites: ";
                cin>>n;
                cout<<"Enter the Number of Requests: ";
                cin>>r;
                StressTesting(n,r);
                break;
            case 6:
                cout<<"Enter the Number of Websites: ";
                cin>>n;
                EmptyQueues(n);
                break;
            
            case 7:

                cout<<"Enter the Number of Websites: ";
                cin>>n;
                StressTesting(n);
                break;
            
            case 8:
                cout<<"Enter the Number of Requests: ";
                cin>>r;
                StressTesting(1,r);
                break;
            case 9:
               manual_input();
               break;
            default:
                cout<<"Exiting the TC Wizard"<<endl;
                exit(0);
        }
    }
}