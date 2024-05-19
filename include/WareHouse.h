#pragma once
#include <string>
#include <vector>

#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Action.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);//Constructor
        WareHouse(const WareHouse &other);//Copy Constructor
        WareHouse &operator=(const WareHouse &other);//Copy Assignment
        WareHouse(WareHouse&& other) noexcept;//Move Constructor
        WareHouse &operator=(WareHouse&& other) noexcept;//Move assigment operator

        ~WareHouse();//Destructor
        void start();
        void addOrder(Order* order);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        void destructPrep();
        vector<Volunteer*> &getVolunteers();
        vector<Order*> &getPendingOrders();
        void moveOrderToInProcess(Order* order);
        void removeDupInProcess();
        void moveOrderToCompleted(Order* order);
        void removeDupCompleted();
        void moveOrderToInPending(Order* order);
        void removeDupInPending();
        int uniqeOrderNum();  
        int uniqeCustomerId();
       
        int getCustomerCounter() const;
        int getOrderCounter() const;
        void AddCustomer(Customer* customer);
        int getVolunteerCounter() const;
        void addAction(BaseAction* action);
        vector<Order*> getAllOrders();
        vector<Customer*> getCustomers();
        void checkActionAct(string input);
        void simulateStep(int numOfSteps, BaseAction* action);
        void addOrder(const int customerId, BaseAction* action);
        void addCustomer(const string &customerName, const string &customerType,  int distance, int maxOrders, BaseAction* action);
        void printOrderStstus(int orderId, BaseAction* action);
        void printCustomerStatus(int customerId, BaseAction* action);
        void printVolunteerStatus(int volunteerId, BaseAction* action);
        bool checkForValidVolunteer(int volunteerId);

        

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
};