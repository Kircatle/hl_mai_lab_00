#include "delivery.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>
#include <future>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace models
{
    void Delivery::init()
    {
        try
        {

            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Statement create_stmt(db_session);
            Statement create_trigger(db_session);
            Statement create_index_sender_id(db_session);
            Statement create_receiver_id(db_session);
            create_stmt 
                        << "create table if not exists delivery"
                        << "("
                        << "id varchar(36) default uuid() primary key,"
                        << "package_id varchar(36) not null,"
                        << "sender_id varchar(36) not null,"
                        << "receiver_id  varchar(36) not null,"
                        << "departure_date datetime,"
                        << "start_point text,"
                        << "destination text,"
                        << "status text"
                        << ");",
                        now;
            std::cout << create_stmt.toString() << std::endl;
            create_trigger 
                        << "CREATE TRIGGER IF NOT EXISTS last_uuiddelivery AFTER INSERT ON delivery FOR EACH ROW SET @last_delivery_uuid = NEW.id;",
                        now;
            std::cout << create_trigger.toString() << std::endl;
            create_index_sender_id
                        << "create index if not exists sender_id on delivery(sender_id);",
                        now;
            std::cout << create_index_sender_id.toString() << std::endl;
            create_receiver_id
                        << "create index if not exists receiver_id on delivery(receiver_id);",
                        now;
            std::cout << create_receiver_id.toString() << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
    }

    Delivery Delivery::from_json(const std::string &json)
    {
        Delivery delivery;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
        delivery.delivery_uuid = object->getValue<std::string>("id");
        delivery.package_uuid = object->getValue<std::string>("package_id");
        delivery.sender_uuid = object->getValue<std::string>("sender_id");
        delivery.receiver_uuid = object->getValue<std::string>("receiver_id");
        delivery.departure_date = object->getValue<std::string>("departure_date");
        delivery.start_point = object->getValue<std::string>("start_point");
        delivery.destination = object->getValue<std::string>("destination");
        delivery.status = object->getValue<std::string>("status");
        return delivery;
    }

    Poco::JSON::Object::Ptr Delivery::to_json() const
    {
        Poco::JSON::Object::Ptr object = new Poco::JSON::Object();
        object->set("id", delivery_uuid);
        object->set("package_id", package_uuid);
        object->set("sender_id", sender_uuid);
        object->set("receiver_id", receiver_uuid);
        object->set("departure_date", departure_date);
        object->set("start_point", start_point);
        object->set("destination", destination);
        object->set("status", status);
        return object;
    }

    std::optional<Delivery> Delivery::get_by_id(std::string &delivery_uuid)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Delivery output;
            select << "select id,package_id,sender_id,receiver_id,departure_date, start_point, destination, status from delivery.delivery where id=?",
                into(output.delivery_uuid),
                into(output.package_uuid),
                into(output.sender_uuid),
                into(output.receiver_uuid),
                into(output.departure_date),
                into(output.start_point),
                into(output.destination),
                into(output.status),
                use(delivery_uuid),
                range(0, 1);

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return output;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
        return {};
    }
    
    std::vector<Delivery> Delivery::get_by_package_uuid(std::string &package_uuid)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Delivery output;
            select << "select id,package_id,sender_id,receiver_id,departure_date, start_point, destination, status from delivery.delivery where package_id=?",
                use(package_uuid),
                now;

            select.execute();
            Poco::Data::RecordSet rs(select);
            std::vector<Delivery> result;
            bool more = true;
            while (more)
            {
                Delivery delivery;
                delivery.delivery_uuid = rs[0].convert<std::string>();
                delivery.package_uuid = rs[1].convert<std::string>();
                delivery.sender_uuid = rs[2].convert<std::string>();
                delivery.receiver_uuid= rs[3].convert<std::string>();
                if (rs[4].isEmpty())
                {
                    delivery.departure_date="";
                }
                else
                {
                    delivery.departure_date = rs[4].convert<std::string>();
                }
                if (rs[5].isEmpty())
                {
                   delivery.start_point = "";
                }
                else
                {
                    delivery.start_point = rs[5].convert<std::string>();
                }
                if ( rs[6].isEmpty())
                {
                    delivery.destination = "";
                }   
                else
                {
                    delivery.destination = rs[6].convert<std::string>();
                }
                if (rs[7].isEmpty())
                {
                    delivery.status = "";
                }
                else
                {
                    delivery.status = rs[7].convert<std::string>();
                }
                result.push_back(delivery);
                more = rs.moveNext();
            }
            return result;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
        return {};
    }

    std::vector<Delivery> Delivery::get_by_sender_uuid(std::string &sender_uuid)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Delivery output;
            select << "select id,package_id,sender_id,receiver_id,departure_date, start_point, destination, status from delivery.delivery where sender_id=?",
                use(sender_uuid),
                now;

            select.execute();
            Poco::Data::RecordSet rs(select);
            std::vector<Delivery> result;
            bool more = true;
            while (more)
            {
                Delivery delivery;
                delivery.delivery_uuid = rs[0].convert<std::string>();
                delivery.package_uuid = rs[1].convert<std::string>();
                delivery.sender_uuid = rs[2].convert<std::string>();
                delivery.receiver_uuid= rs[3].convert<std::string>();
                if (rs[4].isEmpty())
                {
                    delivery.departure_date="";
                }
                else
                {
                    delivery.departure_date = rs[4].convert<std::string>();
                }
                if (rs[5].isEmpty())
                {
                   delivery.start_point = "";
                }
                else
                {
                    delivery.start_point = rs[5].convert<std::string>();
                }
                if ( rs[6].isEmpty())
                {
                    delivery.destination = "";
                }   
                else
                {
                    delivery.destination = rs[6].convert<std::string>();
                }
                if (rs[7].isEmpty())
                {
                    delivery.status = "";
                }
                else
                {
                    delivery.status = rs[7].convert<std::string>();
                }
                result.push_back(delivery);
                more = rs.moveNext();
            }
            return result;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
        return {};
    }

    std::vector<Delivery> Delivery::get_by_receiver_uuid(std::string &receiver_uuid)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Delivery output;
            select << "select id,package_id,sender_id,receiver_id,departure_date, start_point, destination, status from delivery.delivery where receiver_id=?",
                use(receiver_uuid),
                now;

            select.execute();
            Poco::Data::RecordSet rs(select);
            std::vector<Delivery> result;
            bool more = true;
            while (more)
            {
                Delivery delivery;
                delivery.delivery_uuid = rs[0].convert<std::string>();
                delivery.package_uuid = rs[1].convert<std::string>();
                delivery.sender_uuid = rs[2].convert<std::string>();
                delivery.receiver_uuid= rs[3].convert<std::string>();
                if (rs[4].isEmpty())
                {
                    delivery.departure_date="";
                }
                else
                {
                    delivery.departure_date = rs[4].convert<std::string>();
                }
                if (rs[5].isEmpty())
                {
                   delivery.start_point = "";
                }
                else
                {
                    delivery.start_point = rs[5].convert<std::string>();
                }
                if ( rs[6].isEmpty())
                {
                    delivery.destination = "";
                }   
                else
                {
                    delivery.destination = rs[6].convert<std::string>();
                }
                if (rs[7].isEmpty())
                {
                    delivery.status = "";
                }
                else
                {
                    delivery.status = rs[7].convert<std::string>();
                }
                delivery.start_point = rs[5].convert<std::string>();
                delivery.destination = rs[6].convert<std::string>();
                delivery.status = rs[7].convert<std::string>();
                result.push_back(delivery);
                more = rs.moveNext();
            }
            std::cout << "result";
            return result;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
        return {};
    }


    void Delivery::save_to_db()
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement insert(session);
            std::cout <<package_uuid << std::endl;
            std::cout <<sender_uuid << std::endl;
            std::cout <<receiver_uuid << std::endl;
            std::cout <<departure_date << std::endl;
            std::cout <<start_point << std::endl;
            std::cout <<destination << std::endl;
            std::cout << status << std::endl;
            insert << "insert into delivery (package_id,sender_id,receiver_id,departure_date, start_point, destination, status) values(?, ?, ?, ?, ?, ?, ?);",
                use(package_uuid),
                use(sender_uuid),
                use(receiver_uuid),
                use(departure_date),
                use(start_point),
                use(destination),
                use(status),
                range(0,1);
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT @last_delivery_uuid;",
                into(delivery_uuid),
                range(0, 1); 

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "Inserted new package with id = " << delivery_uuid << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
    }

    std::string Delivery::get_delivery_uuid() const
    {
        return delivery_uuid;
    }

    std::string Delivery::get_package_uuid() const
    {
        return package_uuid;
    }
    std::string Delivery::get_sender_uuid() const
    {
        return sender_uuid;
    }
    std::string Delivery::get_receiver_uuid() const
    {
        return receiver_uuid;
    }

    std::string Delivery::get_departure_date() const
    {
        return departure_date;
    }

    std::string Delivery::get_start_point() const
    {
        return start_point;
    }

    std::string Delivery::get_destination() const
    {
        return destination;
    }

    std::string Delivery::get_status() const
    {
        return status;
    }

    void Delivery::set_package_uuid(std::string pkg_uuid)
    {
        package_uuid = pkg_uuid;
    }

    void Delivery::set_sender_uuid(std::string send_uuid)
    {
        sender_uuid = send_uuid;
    }

    void Delivery::set_receiver_uuid(std::string rcv_uuid)
    {
        receiver_uuid = rcv_uuid;
    }

    void Delivery::set_departure_date(std::string dpt_date)
    {
        departure_date = dpt_date;
    }

    void Delivery::set_start_point(std::string sp)
    {
        start_point = sp;
    }
    void Delivery::set_destination(std::string dest)
    {
        destination = dest;
    }
    void Delivery::set_status(std::string _status)
    {
        status = _status;
    }
}