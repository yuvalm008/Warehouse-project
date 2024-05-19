#include "../include/Volunteer.h"
#include <iostream>

Volunteer::Volunteer(int id, const string &name): completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

const string &Volunteer::getName() const{
    return name;
}

int Volunteer::getId() const{
    return id;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    return activeOrderId != -1;
}

bool Volunteer::hasOrdersLeft() const{
    return true;
}

bool Volunteer::canTakeOrder(const Order &order) const{
    return false;
}

void Volunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();

}

void Volunteer::step(){}

string Volunteer::toString() const{
    std::string ret = "\nVolunteerId: " + std:: to_string(getId());
    if (isBusy())
    {      
        ret += "\nisBusy: TRUE";
        ret += "\nOrderId: " + std:: to_string(getActiveOrderId());
    }
    else
    {
        ret += "\nisBusy: FALSE";
        ret += "\nOrderId: None";
    }
    ret += "\nordersLeft: No Limit";
    return ret;
}

Volunteer* Volunteer::clone() const{
    return nullptr;
};

Volunteer::~Volunteer(){}

Volunteer::Volunteer(const Volunteer &other): completedOrderId(other.completedOrderId), activeOrderId(other.activeOrderId), id(other.id), name(other.name){}


///////////////////////////////////CollectorVolunteer////////////////////////////////////////////

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown): Volunteer(id,name), coolDown(coolDown), timeLeft(0){}

void CollectorVolunteer::step(){
    if(isBusy()){
        bool isDone = decreaseCoolDown();
        if(isDone){
            completedOrderId = activeOrderId;
            activeOrderId = -1;
        }
    }
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::PENDING && !isBusy())
    {
        return true;
    }
    else
    {
        return false;
    }    
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    timeLeft--;
    return (timeLeft==0);
}

void CollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    timeLeft = coolDown;   
}

bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}

string CollectorVolunteer::toString() const{
    std:: string ret = "\nVolunteerId: " + std:: to_string(getId());
    if (isBusy())
    {      
        ret += "\nisBusy: TRUE";
        ret += "\nOrderId: " + std:: to_string( getActiveOrderId());
        ret += "\nTimeLeft: " + std:: to_string(getTimeLeft());
    }
    else
    {
        ret += "\nisBusy: FALSE";
        ret +="\nOrderId: None";
        ret += "\nTimeLeft: None";
    }
    ret += "\nordersLeft: No Limit";
    return ret;
}

CollectorVolunteer* CollectorVolunteer::clone() const{
    return new CollectorVolunteer(*this);
}



CollectorVolunteer::~CollectorVolunteer() {}


CollectorVolunteer::CollectorVolunteer(const CollectorVolunteer &other): Volunteer(other), coolDown(other.coolDown), timeLeft(other.timeLeft){}



//////////////////////////////////LimitedCollectorVolunteer/////////////////////////////////////////////

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders):
                             CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    return ordersLeft>0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::PENDING && getTimeLeft()==0 && !isBusy() && getNumOrdersLeft()>0)
    {
        return true;
    }
    else
    {
        return false;
    }    
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    ordersLeft--;
    CollectorVolunteer:: acceptOrder(order);
    }

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const{
    std::string ret = "\nVolunteerId: " +std:: to_string(getId()) ;
    if (isBusy())
    {      
        ret += "\nisBusy: TRUE";
        ret += "\nOrderId: " + std:: to_string(getActiveOrderId());
        ret += "\nTimeLeft: " + std:: to_string(getTimeLeft());
    }
    else
    {
        ret += "\nisBusy: FALSE";
        ret += "\nOrderId: None";
        ret += "\nTimeLeft: None";
    }
    ret += "\nOrdersLeft: " + std:: to_string(getNumOrdersLeft());
    return ret;
}


LimitedCollectorVolunteer::~LimitedCollectorVolunteer() {}


LimitedCollectorVolunteer::LimitedCollectorVolunteer(const LimitedCollectorVolunteer &other): CollectorVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft){}

//////////////////////////////////DriverVolunteer/////////////////////////////////////////////

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
                        Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0){
                        }

void DriverVolunteer::step(){
    if(isBusy()){
        bool isDone = decreaseDistanceLeft();
        if(isDone){
            completedOrderId = activeOrderId;
            activeOrderId = -1;
        }
    }
}

bool DriverVolunteer::canTakeOrder(const Order &order) const{
    return(order.getStatus() == OrderStatus::COLLECTING  && !isBusy() && order.getDistance()<=maxDistance);
}

int DriverVolunteer:: getDistanceLeft() const{
    return distanceLeft;
}

bool DriverVolunteer:: decreaseDistanceLeft(){
    distanceLeft = distanceLeft-distancePerStep;
    if(distanceLeft<0){
        distanceLeft=0;
    }
    return (distanceLeft==0);
}

void DriverVolunteer:: acceptOrder(const Order &order){
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

bool DriverVolunteer:: hasOrdersLeft() const{
    return true;
}

string DriverVolunteer:: toString() const{
    std::string ret = "\nVolunteerId: " + std:: to_string(getId());
    if (isBusy())
    {      
        ret += "\nisBusy: TRUE";
        ret += "\nOrderId: " + std:: to_string(getActiveOrderId());
        ret += "\nDistanceLeft: " + std:: to_string(getDistanceLeft());
    }
    else
    {
        ret += "\nisBusy: FALSE";
        ret += "\nOrderId: None";
        ret += "\nDistanceLeft: None";
    }
        ret += "\nordersLeft: No Limit";

    return ret;
}

DriverVolunteer* DriverVolunteer:: clone() const{
    return new DriverVolunteer(*this);
    }

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}


DriverVolunteer::~DriverVolunteer() {}

DriverVolunteer::DriverVolunteer(const DriverVolunteer &other): Volunteer(other), maxDistance(other.maxDistance), distancePerStep(other.distancePerStep), distanceLeft(other.distanceLeft){}


//////////////////////////////////LimitedDriverVolunteer/////////////////////////////////////////////

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
                        DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders){}
          
LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::COLLECTING  && !isBusy() && order.getDistance() <= getMaxDistance() && hasOrdersLeft())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){
    ordersLeft--;
    DriverVolunteer::acceptOrder(order);
}

string LimitedDriverVolunteer::toString() const{
    std::string ret = "\nVolunteerId: " + std:: to_string(getId());
    if (isBusy())
    {      
        ret += "\nisBusy: TRUE";
        ret += "\nOrderId: " + std:: to_string(getActiveOrderId());
        ret += "\nDistanceLeft: " +std:: to_string( getDistanceLeft());
    }
    else
    {
        ret += "\nisBusy: FALSE";
        ret += "\nOrderId: None";
        ret += "\nDistanceLeft: None";
    }
    ret += "\nOrdersLeft: " +std:: to_string(getNumOrdersLeft());
    return ret;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft>0;
}






LimitedDriverVolunteer::~LimitedDriverVolunteer() {}

LimitedDriverVolunteer::LimitedDriverVolunteer(const LimitedDriverVolunteer &other): DriverVolunteer(other), maxOrders(other.maxOrders), ordersLeft(other.ordersLeft){}