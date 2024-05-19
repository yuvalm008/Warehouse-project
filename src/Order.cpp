#include "../include/Order.h"
#include <iostream>

Order::Order(int id, int customerId, int distance): id(id), customerId(customerId),distance(distance), status(OrderStatus::PENDING), collectorId(-1), driverId(-1) {}

int Order::getId() const{
    return id;
}

Order* Order::clone() const{
    return new Order(*this);
}

int Order::getDistance() const{
    return distance;
}

int Order::getCustomerId() const{
    return customerId;
}

void Order::setStatus(OrderStatus status){
    this->status = status;
}       

void Order::setCollectorId(int collectorId){
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId){
    this->driverId = driverId;
}

int Order::getCollectorId() const{
    return collectorId;
}

int Order::getDriverId() const{
    return driverId;
}

OrderStatus Order::getStatus() const{
    return status;
}

const string Order::toString() const{
    string ret = "OrderId: " + std::to_string(id) + "\nOrderStatus: " + getStatusString() + "\nCustomerId " + std::to_string(customerId) +
             "\nCollector: " ;
    if(collectorId == -1){
        ret += " None";
    }
    else{
        ret += std::to_string(collectorId);
    }

    ret += "\nDriver: ";
    if(driverId == -1){
        ret+= "None";
    }
    else{
        ret += std::to_string(driverId);
    }
    return ret;
}

const string Order::getStatusString() const{
    string statusString;
    switch (status)
    {
    case OrderStatus::PENDING:
        statusString = "Pending";
        break;
    case OrderStatus::COLLECTING:
        statusString = "Collecting";
        break;
    case OrderStatus::DELIVERING:
        statusString = "Delivering";
        break;
    case OrderStatus::COMPLETED:
        statusString = "Completed";
        break;
    default:
        break;
    }
    return statusString;
}


// Copy constructor
Order::Order(const Order& other): id(other.id), customerId(other.customerId), distance(other.distance), status(other.status), collectorId(other.collectorId), driverId(other.driverId) {}




