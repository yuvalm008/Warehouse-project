#include "../include/WareHouse.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(), volunteers(), pendingOrders(),
                                                     inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0)
{
    std::ifstream file(configFilePath);
    if (!file)
    {
        throw std::runtime_error("Could not open file");
    }
    customerCounter = 0;
    volunteerCounter = 0;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue; 
        }
        else
        {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "customer")
            {
                Customer *customer;
                std::string name;
                std::string customerType;
                int distance;
                int maxOrders;
                iss >> name >> customerType >> distance >> maxOrders;
                int id = uniqeCustomerId();
                if (customerType == "soldier")
                {
                    customer = new SoldierCustomer(id, name, distance, maxOrders);
                    customers.push_back(customer);
                }
                else if (customerType == "civilian")
                {
                    customer = new CivilianCustomer(id, name, distance, maxOrders);
                    customers.push_back(customer);
                }
            }
            else if (type == "volunteer")
            {
                Volunteer *volunteer;
                volunteerCounter = volunteers.size();
                std::string name, role;
                int coolDown, maxDistance, distancePerStep, maxOrders;
                iss >> name >> role;
                if (role == "collector" || role == "limited_collector")
                {
                    iss >> coolDown;
                    if (role == "limited_collector")
                    {
                        iss >> maxOrders;
                        volunteer = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);
                        volunteers.push_back(volunteer);
                    }
                    else
                    {
                        volunteer = new CollectorVolunteer(volunteerCounter, name, coolDown);
                        volunteers.push_back(volunteer);
                    }
                }
                else if (role == "driver" || role == "limited_driver")
                {
                    iss >> maxDistance >> distancePerStep;
                    if (role == "limited_driver")
                    {
                        iss >> maxOrders;
                        volunteer = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
                        volunteers.push_back(volunteer);
                    }
                    else
                    {
                        volunteer = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
                        volunteers.push_back(volunteer);
                    }
                }
            }
        }
    }
    file.close();
}

WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(),
                                               inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter)
{
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
    
    for (Volunteer *volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
    }
     
    for (Order *order : other.pendingOrders)
    {
        pendingOrders.push_back(order->clone());
    }
     
    for (Order *order : other.inProcessOrders)
    {
        inProcessOrders.push_back(order->clone());
    }
     
    for (Order *order : other.completedOrders)
    {
        completedOrders.push_back(order->clone());
    }
    
    for (Customer *customer : other.customers)
    {
        customers.push_back(customer->clone());
    }
}

WareHouse &WareHouse::operator=(const WareHouse &other)
{

    if (this != &other)
    {
        destructPrep();
        isOpen = other.isOpen;
         
        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }
         
        for (Volunteer *volunteer : other.volunteers)
        {
            volunteers.push_back(volunteer->clone());
        }
         
        for (Order *order : other.pendingOrders)
        {
            pendingOrders.push_back(order->clone());
        }
         
        for (Order *order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order->clone());
        }
         
        for (Order *order : other.completedOrders)
        {
            completedOrders.push_back(order->clone());
        }
        for (Customer *customer : other.customers)
        {
            customers.push_back(customer->clone());
        }
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
    }
    return *this;
}

WareHouse &WareHouse::operator=(WareHouse&& other) noexcept{
    if (this != &other)
    {
        destructPrep();
        isOpen = other.isOpen;
         
        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action);
        }
        other.actionsLog.clear();
        for (Volunteer *volunteer : other.volunteers)
        {
            volunteers.push_back(volunteer);
        }
        other.volunteers.clear();
        for (Order *order : other.pendingOrders)
        {
            pendingOrders.push_back(order);
        } 
        other.pendingOrders.clear();
        for (Order *order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order);
        }
        other.inProcessOrders.clear();
        for (Order *order : other.completedOrders)
        {
            completedOrders.push_back(order);
        }
        other.completedOrders.clear();
        for (Customer *customer : other.customers)
        {
            customers.push_back(customer);
        }
        other.customers.clear();
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        
    }
    return *this;

}


void WareHouse::destructPrep()
{
    for (auto customer = customers.begin(); customer != customers.end();)
    {
        delete *customer;
        customer = customers.erase(customer);
    }
    customers.clear();

    for (auto volunteer = volunteers.begin(); volunteer != volunteers.end();)
    {
        delete *volunteer;
        volunteer = volunteers.erase(volunteer);
    }
    volunteers.clear();
    
    for (auto order = pendingOrders.begin(); order != pendingOrders.end();)
    {
        delete *order;
        order = pendingOrders.erase(order);
    }
    pendingOrders.clear();

    for (auto order = inProcessOrders.begin(); order != inProcessOrders.end();)
    {
        delete *order;
        order = inProcessOrders.erase(order);
    }
    inProcessOrders.clear();

    for (auto order = completedOrders.begin(); order != completedOrders.end();)
    {
        delete *order;
        order = completedOrders.erase(order);
    }
    completedOrders.clear();
    for (auto action = actionsLog.begin(); action != actionsLog.end();)
    {
        delete *action;
        action = actionsLog.erase(action);
    }
    actionsLog.clear();
}


WareHouse::WareHouse(WareHouse&& other) noexcept
    : isOpen(std::move(other.isOpen)),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(std::move(other.customerCounter)),
      volunteerCounter(std::move(other.volunteerCounter))
{
    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();
}




WareHouse::~WareHouse()
{

    for (auto customer = customers.begin(); customer != customers.end();)
    {
        delete *customer;
        customer = customers.erase(customer);
    }
    customers.clear();

    for (auto volunteer = volunteers.begin(); volunteer != volunteers.end();)
    {
        delete *volunteer;
        volunteer = volunteers.erase(volunteer);
    }
    volunteers.clear();

    for (auto order = pendingOrders.begin(); order != pendingOrders.end();)
    {
        delete *order;
        order = pendingOrders.erase(order);
    }

    pendingOrders.clear();

    for (auto order = inProcessOrders.begin(); order != inProcessOrders.end();)
    {
        delete *order;
        order = inProcessOrders.erase(order);
    }
    inProcessOrders.clear();

    for (auto order = completedOrders.begin(); order != completedOrders.end();)
    {
        delete *order;
        order = completedOrders.erase(order);
    }
    completedOrders.clear();

    for (auto action = actionsLog.begin(); action != actionsLog.end();)
    {
        delete *action;
        action = actionsLog.erase(action);
    }
    actionsLog.clear();
}

void WareHouse::start()
{
    open();
    string input;
    std::getline(std::cin, input);
    while (input != "close")
    {
        checkActionAct(input); // מתרגם לפעולות ונקרא לו בשם אחר
        std::getline(std::cin, input);
    }
    BaseAction *closeAction = new Close();
    addAction(closeAction);
    closeAction->act(*this);
}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

void WareHouse::checkActionAct(string input)
{
    std::istringstream iss(input);
    string action;
    iss >> action;

    if (action == "order")
    {
        string name;
        string address;
        int id;
        iss >> id;
        BaseAction *orderAction = new ::AddOrder(id);
        orderAction->act(*this);
        addAction(orderAction);
    }
    else if (action == "customer")
    {
        string name;
        string type;
        int distance;
        int maxOrders;
        iss >> name >> type >> distance >> maxOrders;
        BaseAction *customersAction = new ::AddCustomer(name, type, distance, maxOrders);
        customersAction->act(*this);
        addAction(customersAction);
    }
    else if (action == "step")
    {
        int numOfSteps;
        iss >> numOfSteps;
        BaseAction *stepAction = new SimulateStep(numOfSteps);
        stepAction->act(*this);
        addAction(stepAction);
    }
    else if (action == "orderStatus")
    {
        int id;
        iss >> id;
        BaseAction *orderStatusAction = new PrintOrderStatus(id);
        orderStatusAction->act(*this);
        addAction(orderStatusAction);
    }
    else if (action == "customerStatus")
    {
        int id;
        iss >> id;
        BaseAction *customersAction = new PrintCustomerStatus(id);
        customersAction->act(*this);
        addAction(customersAction);
    }
    else if (action == "volunteerStatus")
    {
        int id;
        iss >> id;
        BaseAction *volunteerStatusAction = new PrintVolunteerStatus(id);
        volunteerStatusAction->act(*this);
        addAction(volunteerStatusAction);
    }

    else if (action == "log")
    {
        BaseAction *logAction = new PrintActionsLog();
        logAction->act(*this);
        addAction(logAction);
    }
    else if (action == "close")
    {
        BaseAction *closeAction = new Close();
        addAction(closeAction);
        closeAction->act(*this);
    }
    else if (action == "backup")
    {
        BaseAction *backupWareHouseAction = new BackupWareHouse();
        backupWareHouseAction->act(*this);
        addAction(backupWareHouseAction);
    }
    else if (action == "restore")
    {
        BaseAction *restoreAction = new RestoreWareHouse();
        restoreAction->act(*this);
        addAction(restoreAction);
    }
    else
    {
        std::cout << "Invalid input" << std::endl;
    }
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (Customer *customer : customers)
    {
        if (customer->getId() == customerId)
        {
            return *customer;
        }
    }
    Customer *customerNotFound = new SoldierCustomer(-1, "unknown", 0, 0); // unreachable
    return *customerNotFound;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    return *volunteers[volunteerId];
}

Order &WareHouse::getOrder(int orderId) const
{
    for (Order *order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    Order *orderNotFound = new Order(-1, -1, -1); // unreachable
    return *orderNotFound;
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
    // todo
}

vector<Order *> &WareHouse::getPendingOrders()
{
    return pendingOrders;
}

void WareHouse::close()
{
    for (Order *order : pendingOrders)
    {
        std::cout << "OrderId: " << order->getId() << ", CustomerId: " << order->getCustomerId() << ", Status: " << order->getStatusString() << std::endl;
    }
    for (Order *order : inProcessOrders)
    {
        std::cout << "OrderId: " << order->getId() << ", CustomerId: " << order->getCustomerId() << ", Status: " << order->getStatusString() << std::endl;
    }
    for (Order *order : completedOrders)
    {
        std::cout << "OrderId: " << order->getId() << ", CustomerId: " << order->getCustomerId() << ", Status: " << order->getStatusString() << std::endl;
    }
    this->isOpen = false;
}

void WareHouse::open()
{
    std::cout << "Warehouse is open!" << std::endl;
    isOpen = true;
}

vector<Volunteer *> &WareHouse::getVolunteers()
{
    return volunteers;
}

void WareHouse::moveOrderToInProcess(Order *order)
{
    if (order->getStatus() == OrderStatus::PENDING)
    {
        order->setStatus(OrderStatus::COLLECTING);
    }
    else if (order->getStatus() == OrderStatus::COLLECTING)
    {
        order->setStatus(OrderStatus::DELIVERING);
    }
    inProcessOrders.push_back(order);
}

void WareHouse::removeDupInPending()
{
    for (Order *order : inProcessOrders)
    {
        for (Order *order2 : pendingOrders)
        {
            if (order->getId() == order2->getId())
            {
                pendingOrders.erase(remove(pendingOrders.begin(), pendingOrders.end(), order), pendingOrders.end());
            }
        }
    }
}

void WareHouse::moveOrderToCompleted(Order *order)
{
    order->setStatus(OrderStatus::COMPLETED);
    completedOrders.push_back(order);
}

void WareHouse::removeDupCompleted()
{
    for (Order *order : completedOrders)
    {
        for (Order *order2 : inProcessOrders)
        {
            if (order->getId() == order2->getId())
            {
                inProcessOrders.erase(remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
            }
        }
    }
}

void WareHouse::moveOrderToInPending(Order *order)
{
    pendingOrders.push_back(order);
}

void WareHouse::removeDupInProcess()
{
    for (Order *order : pendingOrders)
    {
        for (Order *order2 : inProcessOrders)
        {
            if (order->getId() == order2->getId())
            {
                inProcessOrders.erase(remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
            }
        }
    }
}

int WareHouse::uniqeOrderNum()
{
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size();
}

int WareHouse::uniqeCustomerId()
{
    customerCounter = customers.size();
    return customerCounter;
}

int WareHouse::getCustomerCounter() const

{
    return customerCounter;
}

int WareHouse::getOrderCounter() const
{
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size();
}

void WareHouse::AddCustomer(Customer *customer)
{
    customers.push_back(customer);
}

int WareHouse::getVolunteerCounter() const
{
    return volunteerCounter;
}

vector<Customer *> WareHouse::getCustomers()

{
    return customers;
}

void WareHouse::simulateStep(int numOfSteps, BaseAction *action)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        for (Order *order : pendingOrders)
        {
            for (Volunteer *volenteer : getVolunteers())
            {
                if (volenteer->canTakeOrder(*order))
                {
                    volenteer->acceptOrder(*order);
                    if (order->getStatus() == OrderStatus::PENDING)
                    {
                        order->setCollectorId(volenteer->getId());
                    }
                    else if (order->getStatus() == OrderStatus::COLLECTING)
                    {
                        order->setDriverId(volenteer->getId());
                    }
                    moveOrderToInProcess(order);
                    break;
                }
            }
        }
        removeDupInPending();

        for (Volunteer *volenteer : getVolunteers())
        {
            volenteer->step();
            int orderId = volenteer->getCompletedOrderId();
            if (!volenteer->isBusy() && orderId != -1)
            {
                Order *order = &getOrder(orderId);
                if (order->getStatus() == OrderStatus::COLLECTING)
                {
                    moveOrderToInPending(order);
                    removeDupInProcess();
                }
                else if (order->getStatus() == OrderStatus::DELIVERING)
                {
                    moveOrderToCompleted(order);
                    removeDupCompleted();
                }
            }
        }
        auto condition = [](Volunteer *volunteer)
        { return !volunteer->hasOrdersLeft() && !volunteer->isBusy(); };

        for (auto it = volunteers.begin(); it != volunteers.end();)
        {
            if (condition(*it))
            {
                delete *it;
                it = volunteers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void WareHouse::addOrder(const int customerId, BaseAction *action)
{
    const int id = uniqeOrderNum();
    Customer *customer = &getCustomer(customerId);
    int distance = customer->getCustomerDistance();
    Order *order = new Order(id, customerId, distance);
    addOrder(order);
    customer->addOrder(id);
}

void WareHouse::addCustomer(const string &customerName, const string &customerType, int distance, int maxOrders, BaseAction *action)
{
    if (customerType == "soldier")
    {
        AddCustomer(new SoldierCustomer(uniqeCustomerId(), customerName, distance, maxOrders));
    }
    else if (customerType == "civilian")
    {
        AddCustomer(new CivilianCustomer(uniqeCustomerId(), customerName, distance, maxOrders));
    }
}

void WareHouse::printOrderStstus(int orderId, BaseAction *action)
{
    Order *order = &getOrder(orderId);
    std::cout << "\n"
              << order->toString() << std::endl;
}

bool WareHouse::checkForValidVolunteer(int volunteerId)
{
    for (Volunteer *volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return true;
        }
    }
    return false;
}