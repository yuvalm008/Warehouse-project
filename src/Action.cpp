#include "../include/Action.h"
#include "../include/Volunteer.h"
#include <iostream>

BaseAction::BaseAction() :errorMsg(""), status()   {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::act(WareHouse& wareHouse) {};

BaseAction::~BaseAction() {}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

string BaseAction::statusToString(ActionStatus status)
{
   
    if (status == ActionStatus::COMPLETED)
    {
        return "COMPLETED";
    }
    else if (status == ActionStatus::ERROR)
    {
        return "ERROR";
    }
    return "";
}

BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {}

//--------------------------------SimulateStep-------------
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}
SimulateStep::SimulateStep(const SimulateStep &other) : numOfSteps(other.numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    wareHouse.simulateStep(numOfSteps, this);
    complete();
}

std::string SimulateStep::toString() const
{
    return std::string("simulateStep ") + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//---------------------------------------------
AddOrder::AddOrder(const int customerId) : customerId(customerId){}

AddOrder::AddOrder(const AddOrder &other) : customerId(other.customerId) {}

void AddOrder::act(WareHouse &wareHouse)
{
    if (customerId > wareHouse.getCustomerCounter() || customerId < 0)
    {
        error("Cannot place this order");
        std::cout << "Error: " <<getErrorMsg() << std::endl; 
    }
    else
    {
        Customer *customer = &wareHouse.getCustomer(customerId);
        if (!(customer->canMakeOrder()))
        {
            error("Cannot place this order");
            std::cout << "Error: "<< getErrorMsg() << std::endl;
        }
        else
        {
            wareHouse.addOrder(customerId, this);
            complete();
        }
    }

}

std::string AddOrder::toString() const
{
    return std::string("order ") + std::to_string(customerId);
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

//---------------------------------------------

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : customerName(customerName), customerType(typeFromString(customerType)), distance(distance), maxOrders(maxOrders), customerId(-1) {}

AddCustomer::AddCustomer(const AddCustomer &other) : customerName(other.customerName), customerType(other.customerType), distance(other.distance), maxOrders(other.maxOrders), customerId(other.customerId) {}

CustomerType AddCustomer::typeFromString(string customerType)
{
    if (customerType == "Soldier")
    {
        return CustomerType::Soldier;
    }
    else if (customerType == "Civilian")
    {
        return CustomerType::Civilian;
    }
    else
    {
        error("Invalid customer type");
        return CustomerType::Civilian;//defual value, will not be used
    }
}

void AddCustomer::act(WareHouse &wareHouse)
{
    Customer *customer;
    customerId = wareHouse.uniqeCustomerId();
    if (customerType == CustomerType::Soldier)
    {
        customer = new SoldierCustomer(customerId , customerName, distance, maxOrders);
    }
    else if (customerType == CustomerType::Civilian)
    {
        customer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
    }
    wareHouse.AddCustomer(customer);
    complete();
}

std::string AddCustomer::toString() const
{
    return std::string("customer ") + std::to_string(customerId);
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

//---------------------------------------------
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

PrintOrderStatus::PrintOrderStatus(const PrintOrderStatus &other) : orderId(other.orderId) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    if (orderId >= wareHouse.getOrderCounter() || orderId < 0)
    {
        error("Order does not exist");
        std::cout << "Error: " <<getErrorMsg() << std::endl;
    }
    else
    {
        wareHouse.printOrderStstus(orderId, this);
        complete();
    }
}

std::string PrintOrderStatus::toString() const
{
    return std::string("orderStatus ") + std::to_string(orderId);
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

//---------------------------------------------
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}
PrintCustomerStatus::PrintCustomerStatus(const PrintCustomerStatus &other) : customerId(other.customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (customerId > wareHouse.getCustomerCounter() || customerId < 0)
    {
        error("Customer does not exist");
        std::cout << "Error: " <<getErrorMsg() << std::endl;
    }
    else{
        Customer *customer = &wareHouse.getCustomer(customerId);
        std::cout << "\nCustomerId: " << customerId << std::endl;
        const vector<int> &ordersIds = customer->getOrdersIds();
        for (size_t i = 0; i < ordersIds.size(); i++)
            {
            Order *order = &wareHouse.getOrder(ordersIds[i]);
            std::cout << "OrderId: " << order->getId() << std::endl;
            std::cout << "OrderStatus: " << order->getStatusString() << std::endl;
        }
        std::cout << "numOrdersLeft: " << customer->getMaxOrders() - customer->getNumOrders() << std::endl;
        complete();
    }
    
}

std::string PrintCustomerStatus::toString() const
{
    return  std::string("customerStatus ") + std::to_string(customerId);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}
//---------------------------------------------

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}
PrintVolunteerStatus::PrintVolunteerStatus(const PrintVolunteerStatus &other) : volunteerId(other.volunteerId) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (!wareHouse.checkForValidVolunteer(volunteerId)) 
    {
        error("Volunteer does not exist");
        std::cout << "Error: " <<getErrorMsg() << std::endl;
    }
    else
    {
        Volunteer *volunteer = &wareHouse.getVolunteer(volunteerId);
        std::cout << volunteer->toString() << std::endl;
        complete();
    }
}

std::string PrintVolunteerStatus::toString() const
{
    return  std::string("volunteerStatus ") + std::to_string(volunteerId);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

//---------------------------------------------
PrintActionsLog::PrintActionsLog() {}
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) {}
void PrintActionsLog::act(WareHouse &wareHouse)
{
    complete();
    for (BaseAction *action : wareHouse.getActions())
    {
        std::cout << action->toString() << " " <<statusToString(action->getStatus()) << std::endl;
    }  
}

std::string PrintActionsLog::toString() const
{
    return  std::string("log ") ;
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

//---------------------------------------------
Close::Close(){}

Close::Close(const Close &other) {}

void Close::act(WareHouse &wareHouse) {
    wareHouse.close();
}

std::string Close::toString() const
{
    return "close";
}

Close *Close::clone() const
{
    return new Close(*this);
}

//---------------------------------------------
BackupWareHouse::BackupWareHouse() {}

BackupWareHouse::BackupWareHouse(const BackupWareHouse &other) {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    extern WareHouse* backup;
    if(backup != nullptr)
    {
        delete backup;
        backup = new WareHouse(wareHouse);
    }
    else
    {
        backup = new WareHouse(wareHouse);
    }   
       complete();
}

std::string BackupWareHouse::toString() const
{
    return "backupWareHouse";
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

//---------------------------------------------
RestoreWareHouse::RestoreWareHouse() {} // RESTORE WAREHOUSE

RestoreWareHouse::RestoreWareHouse(const RestoreWareHouse &other) {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    extern WareHouse* backup;
    if (backup == nullptr)
    {
        error("No backup available");
        std::cout<< "Error: "<< getErrorMsg() << std::endl;
    }
    else{
            
            wareHouse = *backup;
            complete();
    }    
}

std::string RestoreWareHouse::toString() const
{
    return "restoreWareHouse ";
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
