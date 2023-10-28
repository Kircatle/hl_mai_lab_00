#include "package.h"
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
    void Package::init()
    {
        try
        {

            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Statement create_stmt(db_session);
            Statement create_trigger(db_session);
            Statement create_index_package_user(db_session);
            create_stmt 
                        << "create table if not exists package"
                        << "("
                        << "id varchar(36) default uuid() primary key,"
                        << "user_id varchar(36)      not null,"
                        << "title text    not null,"
                        << "width  double precision not null,"
                        << "height double precision not null,"
                        << "length double precision not null,"
                        << "weight double precision not null,"
                        << "description text"
                        << ");",
                        now;
            std::cout << create_stmt.toString() << std::endl;
            create_trigger 
                        << "CREATE TRIGGER IF NOT EXISTS last_uuidpackage AFTER INSERT ON package FOR EACH ROW SET @last_package_uuid = NEW.id;",
                        now;
            std::cout << create_trigger.toString() << std::endl;
            create_index_package_user
                        << "create index if not exists package_user on package(user_id);",
                        now;
            std::cout << create_index_package_user.toString() << std::endl;
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

    Package Package::from_json(const std::string &json)
    {
        Package package;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        package.package_uuid = object->getValue<std::string>("id");
        package.user_uuid = object->getValue<std::string>("user_id");
        package.title = object->getValue<std::string>("title");
        package.width = object->getValue<double>("width");
        package.height = object->getValue<double>("height");
        package.length = object->getValue<double>("length");
        package.weight = object->getValue<double>("weight");
        package.description = object->getValue<std::string>("description");
        return package;
    }

    Poco::JSON::Object::Ptr Package::to_json() const
    {
        Poco::JSON::Object::Ptr object = new Poco::JSON::Object();

        object->set("id", package_uuid);
        object->set("user_id", user_uuid);
        object->set("title", title);
        object->set("width", width);
        object->set("height", height);
        object->set("length", length);
        object->set("weight", weight);
        object->set("description", description);
        return object;
    }

    std::optional<Package> Package::get_by_id(std::string &package_uuid)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Package output;
            select << "select id, user_id,title,width,height,length,weight,description from package where id=?",
                into(output.package_uuid),
                into(output.user_uuid),
                into(output.title),
                into(output.width),
                into(output.height),
                into(output.length),
                into(output.weight),
                into(output.description),
                use(package_uuid),
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
    
    std::vector<Package> Package::get_by_user_uuid(std::string &user_uuid)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Package output;
            select << "select id, user_id,title,width,height,length,weight,description from package where user_id=?",
                use(user_uuid),
                now;

            select.execute();
            Poco::Data::RecordSet rs(select);
            std::vector<Package> result;
            bool more = true;
            while (more)
            {
                Package package;
                package.package_uuid = rs[0].convert<std::string>();
                package.user_uuid = rs[1].convert<std::string>();
                package.title = rs[2].convert<std::string>();
                package.width= rs[3].convert<double>();
                package.height = rs[4].convert<double>();
                package.length = rs[5].convert<double>();
                package.weight = rs[6].convert<double>();
                package.description = rs[7].convert<std::string>();
                result.push_back(package);
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

    void Package::save_to_db()
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement insert(session);
            insert << "insert into package (user_id,title,width,height,length,weight,description) values(?, ?, ?, ?, ?, ?, ?);",
                use(user_uuid),
                use(title),
                use(width),
                use(height),
                use(length),
                use(weight),
                use(description),
                range(0,1);
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT @last_package_uuid;",
                into(package_uuid),
                range(0, 1); 

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "Inserted new package with id = " << package_uuid << std::endl;
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

    std::string Package::get_package_uuid() const
    {
        return package_uuid;
    }

    std::string Package::get_user_uuid() const
    {
        return user_uuid;
    }
    std::string Package::get_title() const
    {
        return title;
    }
    double Package::get_width() const
    {
        return width;
    }

    double Package::get_height() const
    {
        return height;
    }

    double Package::get_length() const
    {
        return length;
    }

    double Package::get_weight() const
    {
        return weight;
    }

    std::string Package::get_description() const
    {
        return description;
    }

    void Package::set_user_uuid(std::string uid)
    {
        user_uuid = uid;
    }

    void Package::set_title(std::string tl)
    {
        title = tl;
    }

    void Package::set_width(double wdh)
    {
        width = wdh;
    }

    void Package::set_height(double hgt)
    {
        height = hgt;
    }

    void Package::set_length(double lgh)
    {
        length = lgh;
    }
    void Package::set_weight(double wgt)
    {
        weight = wgt;
    }
    void Package::set_description(std::string desc)
    {
        description = desc;
    }
}