#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <utility>
#include <cstdlib>

using namespace std;

// Type aliases for convenience
typedef long long int ll;
typedef pair<int, int> Request; // {request_id, processing_time}

// Website class
class Website
{
public:
    int id;                       // Unique ID of the website
    int owner_id;                 // Owner ID of the website
    int bandwidth;                // Bandwidth of the website
    int processing_power;         // Processing power of the website
    int total_weight;             // Total weight = bandwidth + processing_power
    queue<Request> request_queue; // Queue to store incoming requests

    // Constructor to initialize the website with given attributes
    Website(int website_id, int owner, int bw, int power)
        : id(website_id), owner_id(owner), bandwidth(bw), processing_power(power)
    {
        total_weight = bandwidth + processing_power; // Calculate total weight
    }

    // Add a request to the website's queue
    void add_request(int request_id, int processing_time)
    {
        request_queue.push({request_id, processing_time});
    }
};

// Global list of websites to store all website instances
vector<Website *> websites;

// HttpRequest class
class HttpRequest
{
public:
    int id;                // Unique ID of the HTTP request
    int target_website_id; // Target website ID for the request
    int processing_time;   // Processing time required for the request

    // Constructor to initialize an HTTP request
    HttpRequest(int request_id, int website_id, int processing_time)
        : id(request_id), target_website_id(website_id), processing_time(processing_time) {}

    // Add the request to the target website's queue
    void enqueue_request()
    {
        for (auto &site : websites)
        {
            if (site->id == target_website_id)
            {
                site->add_request(id, processing_time);
                cout << "Request ID: " << id
                     << " added to Website ID: " << target_website_id
                     << " (Bandwidth: " << site->bandwidth
                     << ", Processing Power: " << site->processing_power
                     << ", Weight: " << site->total_weight
                     << ") with Processing Time: " << processing_time << "\n";
                return;
            }
        }
        // If the target website is not found, print an error message
        cerr << "Error: Website with ID " << target_website_id << " not found.\n";
    }
};

// LoadBalancer class
class LoadBalancer
{
public:
    // Add a new website to the load balancer
    void add_website(int website_id, int owner_id, int bandwidth, int processing_power)
    {
        websites.push_back(new Website(website_id, owner_id, bandwidth, processing_power));
    }

    // Process requests using Weighted Fair Queuing (WFQ) algorithm
    void WFQ_Scheduling()
    {
        map<int, double> weights; // Map to store normalized weights of websites
        double total_weight = 0.0;

        // Calculate total weight for all websites
        for (auto &site : websites)
        {
            weights[site->id] = site->total_weight;
            total_weight += site->total_weight;
        }

        // Display weights for all websites
        cout << "\nWebsite Weights (Normalized):\n";
        for (auto &site : websites)
        {
            cout << "Website ID: " << site->id
                 << " (Bandwidth: " << site->bandwidth
                 << ", Processing Power: " << site->processing_power
                 << ", Total Weight: " << site->total_weight << ")\n";
        }

        // Normalize the weights for each website
        for (auto &[id, weight] : weights)
        {
            weight /= total_weight;
        }

        // Schedule requests based on virtual finish times
        multiset<pair<double, Request>> schedule; // {virtual_finish_time, {request_id, processing_time}}
        for (auto &site : websites)
        {
            double virtual_time = 0.0;
            while (!site->request_queue.empty())
            {
                auto [request_id, processing_time] = site->request_queue.front();
                site->request_queue.pop();

                // Calculate the virtual finish time for the request
                virtual_time += processing_time / weights[site->id];
                schedule.insert({virtual_time, {request_id, site->id}});
            }
        }

        // Process requests in order of their virtual finish times
        ll actual_time = 0;
        for (auto &[virtual_time, task] : schedule)
        {
            int request_id = task.first;
            int website_id = task.second;

            // Increment actual processing time based on the website's processing power
            actual_time += websites[website_id - 1]->processing_power;

            cout << "Processed Request ID: " << request_id
                 << " for Website ID: " << website_id
                 << " at Actual Time: " << actual_time << "\n";
        }
    }

    // Clear all websites from the load balancer
    void clear_websites()
    {
        for (auto site : websites)
        {
            delete site; // Free memory
        }
        websites.clear();
    }
};

// LoadBalancer instance
LoadBalancer load_balancer;

// Simulate scenario with equal bandwidth and processing power
void simulate_equal_bandwidth_processing(int num_websites = 3, int num_requests = 10)
{
    // Add websites with equal bandwidth and processing power
    for (int i = 1; i <= num_websites; ++i)
    {
        load_balancer.add_website(i, i, 50, 50);
    }

    // Add requests evenly distributed among websites
    for (int i = 1; i <= num_requests; ++i)
    {
        HttpRequest(i, (i % num_websites) + 1, 25).enqueue_request();
    }

    cout << "Simulating Equal Bandwidth and Processing Power...\n";
    load_balancer.WFQ_Scheduling();
    load_balancer.clear_websites();
}

// Simulate scenario with varied bandwidth
void simulate_varied_bandwidth(int num_websites = 3, int num_requests = 10)
{
    srand(43);
    for (int i = 1; i <= num_websites; ++i)
    {
        load_balancer.add_website(i, i, rand() % 200 + 1, 50);
    }

    for (int i = 1; i <= num_requests; ++i)
    {
        HttpRequest(i, (i % num_websites) + 1, 25).enqueue_request();
    }

    cout << "Simulating Varied Bandwidth...\n";
    load_balancer.WFQ_Scheduling();
    load_balancer.clear_websites();
}

// Simulate scenario with varied processing power
void simulate_varied_processing_power(int num_websites = 3, int num_requests = 10)
{
    srand(43);
    for (int i = 1; i <= num_websites; ++i)
    {
        load_balancer.add_website(i, i, 50, rand() % 100 + 1);
    }

    for (int i = 1; i <= num_requests; ++i)
    {
        HttpRequest(i, (i % num_websites) + 1, 25).enqueue_request();
    }

    cout << "Simulating Varied Processing Power...\n";
    load_balancer.WFQ_Scheduling();
    load_balancer.clear_websites();
}

// Simulate stress test with random attributes
void simulate_stress_test(int num_websites, int num_requests)
{
    srand(43);
    for (int i = 1; i <= num_websites; ++i)
    {
        load_balancer.add_website(i, i, rand() % 200 + 1, rand() % 100 + 1);
    }

    for (int i = 1; i <= num_requests; ++i)
    {
        HttpRequest(i, (i % num_websites) + 1, rand() % 50 + 1).enqueue_request();
    }

    cout << "Simulating Stress Test...\n";
    load_balancer.WFQ_Scheduling();
    load_balancer.clear_websites();
}

// Allow manual input for testing
void manual_input()
{
    int num_websites, num_requests;

    cout << "Enter the number of websites: ";
    cin >> num_websites;

    cout << "Enter website details (id, owner_id, bandwidth, processing_power):\n";
    for (int i = 0; i < num_websites; ++i)
    {
        int id, owner, bandwidth, processing_power;
        cin >> id >> owner >> bandwidth >> processing_power;
        load_balancer.add_website(id, owner, bandwidth, processing_power);
    }

    cout << "Enter the number of requests: ";
    cin >> num_requests;

    cout << "Enter request details (request_id, website_id, processing_time):\n";
    for (int i = 0; i < num_requests; ++i)
    {
        int req_id, website_id, processing_time;
        cin >> req_id >> website_id >> processing_time;
        HttpRequest(req_id, website_id, processing_time).enqueue_request();
    }

    cout << "Processing requests using WFQ...\n";
    load_balancer.WFQ_Scheduling();
    load_balancer.clear_websites();
}

int main()
{
    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Equal Bandwidth and Processing\n";
        cout << "2. Varied Bandwidth\n";
        cout << "3. Varied Processing Power\n";
        cout << "4. Stress Test\n";
        cout << "5. Manual Input\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            simulate_equal_bandwidth_processing();
            break;
        case 2:
            simulate_varied_bandwidth();
            break;
        case 3:
            simulate_varied_processing_power();
            break;
        case 4:
        {
            int num_websites, num_requests;
            cout << "Enter number of websites: ";
            cin >> num_websites;
            cout << "Enter number of requests: ";
            cin >> num_requests;
            simulate_stress_test(num_websites, num_requests);
            break;
        }
        case 5:
            manual_input();
            break;
        case 6:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
