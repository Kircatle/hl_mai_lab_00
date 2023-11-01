#include "user.h"
#include "database.h"
#include "../config/config.h"
#include "../helpers/get_hash.h"
#include "cache.h"
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>
#include <future>
#include <cppkafka/cppkafka.h>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace models
{
    void User::init()
    {
        try
        {

            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
           
            std::vector<std::string> shardings = database::Database::get_all_sharding_hints();
            for (std::string& shard : shardings) {
                    std::cout << "\n"+shard+"\n";
                    Statement create_stmt(db_session);
                    Statement create_trigger(db_session);
                    Statement create_index_user_first_name_index(db_session);
                    Statement create_index_user_last_name_index(db_session);
                    Statement create_index_user_email_index(db_session);
                    create_stmt 
                                << "create table if not exists user"
                                << "("
                                << "id varchar(36) default uuid() primary key,"
                                << "login text not null,"
                                << "password text not null,"
                                << "first_name text not null,"
                                << "last_name text not null,"
                                << "email text,"
                                << "title text,"
                                << "UNIQUE (login),"
                                << "UNIQUE (email)"
                                << ");"
                                << shard,
                                now;
                    std::cout << create_stmt.toString() << std::endl;
                    create_trigger 
                                << "CREATE TRIGGER IF NOT EXISTS last_uuiduser "
                                << "AFTER INSERT ON user "
                                << "FOR EACH ROW "
                                << "SET @last_user_uuid = NEW.id;"
                                << shard,
                                now;
                    std::cout << create_trigger.toString() << std::endl;
                    create_index_user_first_name_index 
                                << "create index if not exists user_first_name_index on user(first_name);"
                                << shard,
                                now;
                    std::cout << create_index_user_first_name_index.toString() << std::endl;
                    create_index_user_last_name_index
                                << "create index if not exists user_last_name_index on user(last_name);"
                                << shard,
                                now;
                    std::cout << create_index_user_last_name_index.toString() << std::endl;
                    create_index_user_email_index
                                << "create index if not exists user_email_index on user(email);"
                                << shard,
                                now;
                    std::cout << create_index_user_email_index.toString() << std::endl;
            }
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

    User User::from_json(std::string &json)
    {
        User user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.user_uuid = object->getValue<std::string>("id");
        user.first_name = object->getValue<std::string>("first_name");
        user.last_name = object->getValue<std::string>("last_name");
        user.login = object->getValue<std::string>("login");
        user.password = object->getValue<std::string>("password");
        user.email = object->getValue<std::string>("email");
        user.title = object->getValue<std::string>("title");
        return user;
    }

    std::optional<User> User::from_cache(const std::string &id)
    {
        try
        {
            std::string result;
            database::Cache cache = database::Cache::get_instance();

            if (cache.get(id, result)) return User::from_json(result);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }

        return {};
    }

    Poco::JSON::Object::Ptr User::to_json() const
    {
        Poco::JSON::Object::Ptr object = new Poco::JSON::Object();

        object->set("id", user_uuid);
        object->set("first_name", first_name);
        object->set("last_name", last_name);
        object->set("login", login);
        object->set("password", password);
        object->set("email", email);
        object->set("title", title);
        return object;
    }
    
    std::optional<User> User::get_by_id(std::string &user_uuid)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            long hash = get_hash(user_uuid);
            
            std::string shard = database::Database::get_sharding_hint(hash);
            Poco::Data::Statement select(session);
            std::cout << hash << shard << user_uuid;
            User output;
            select << "select id, first_name, last_name, login, password, email, title from user where id=?;" << shard,
                into(output.user_uuid),
                into(output.first_name),
                into(output.last_name),
                into(output.login),
                into(output.password),
                into(output.email),
                into(output.title),
                use(user_uuid),
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


    std::optional<User> User::get_by_login(std::string &login)
    {
        Poco::Data::Session session = database::Database::get_instance().create_database_session();
        for (const auto& shard : database::Database::get_all_sharding_hints())
        {
            try
            {
                Poco::Data::Statement select(session);
                User output;
                select << "select id, first_name, last_name, login, password, email, title from user where login=?;" + shard,
                    into(output.user_uuid),
                    into(output.first_name),
                    into(output.last_name),
                    into(output.login),
                    into(output.password),
                    into(output.email),
                    into(output.title),
                    use(login),
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
        }
        return {};
    }

    std::optional<User> User::get_by_email(std::string &email)
    {
        Poco::Data::Session session = database::Database::get_instance().create_database_session();
        for (const auto& shard : database::Database::get_all_sharding_hints())
        {
            try
            {
                Poco::Data::Statement select(session);
                User output;
                select << "select id, first_name, last_name, login, password, email, title from user where email=?;" + shard,
                    into(output.user_uuid),
                    into(output.first_name),
                    into(output.last_name),
                    into(output.login),
                    into(output.password),
                    into(output.email),
                    into(output.title),
                    use(email),
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
        }
        return {};
    }

    std::optional<User> User::auth_by_login(std::string &login, std::string &password)
    {
        for (const auto& shard : database::Database::get_all_sharding_hints())
        {
            try
            {
                Poco::Data::Session session = database::Database::get_instance().create_database_session();
                Poco::Data::Statement select(session);
                User output;
                select << "select id, first_name, last_name, login, password, email, title from user where login=? and password=?;"+ shard,
                    into(output.user_uuid),
                    into(output.first_name),
                    into(output.last_name),
                    into(output.login),
                    into(output.password),
                    into(output.email),
                    into(output.title),
                    use(login),
                    use(password),
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
        }
        return {};
    }
    
   
    std::optional<User> User::auth_by_email(std::string &email, std::string &password)
    {
        for (const auto& shard : database::Database::get_all_sharding_hints())
        {
            try
            {
                Poco::Data::Session session = database::Database::get_instance().create_database_session();
                Poco::Data::Statement select(session);
                User output;
                select << "select id, first_name, last_name, login, password, email, title from user where email=? and password=?;"+shard,
                    into(output.user_uuid),
                    into(output.first_name),
                    into(output.last_name),
                    into(output.login),
                    into(output.password),
                    into(output.email),
                    into(output.title),
                    use(email),
                    use(password),
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
        }
        return {};
    }

    std::vector<User> User::find_by_login(std::string &login)
    {
        std::vector<User> result;
        login += "%";
        for (const auto& shard : database::Database::get_all_sharding_hints())
        {
            try
            {
                Poco::Data::Session session = database::Database::get_instance().create_database_session();
                Statement select(session);
                User user;
                select << "select id, first_name, last_name, login, password, email, title from user where login like ?;" + shard,
                        use(login),
                        now;
                select.execute();
                Poco::Data::RecordSet rs(select);
                bool more = rs.moveFirst();
                while (more)
                {
                    User user;
                    user.user_uuid = rs[0].convert<std::string>();
                    user.first_name = rs[1].convert<std::string>();
                    user.last_name = rs[2].convert<std::string>();
                    user.login = rs[3].convert<std::string>();
                    user.password = rs[4].convert<std::string>();
                    user.email = rs[5].convert<std::string>();
                    user.title = rs[6].convert<std::string>();
                    result.push_back(user);
                    more = rs.moveNext();
                }
                
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
            catch(std::exception &e)
            {
                std::cout << "Database statement exception:" << e.what() << std::endl;
                throw;
            }
        }
        if (result.empty())
        {
            return {};
        }
        return result;
    }

    std::vector<User> User::find_by_email(std::string &email)
    {
        std::vector<User> result;
        email += "%";
        for (const auto& shard : database::Database::get_all_sharding_hints())
        {
            try
            {
                Poco::Data::Session session = database::Database::get_instance().create_database_session();
                Statement select(session);
                User user;

                select << "select id, first_name, last_name, login, password, email, title from user where email like ?;" + shard,
                        use(email),
                        now;

                select.execute();
                Poco::Data::RecordSet rs(select);
                bool more = rs.moveFirst();
                while (more)
                {
                    User user;
                    user.user_uuid = rs[0].convert<std::string>();
                    user.first_name = rs[1].convert<std::string>();
                    user.last_name = rs[2].convert<std::string>();
                    user.login = rs[3].convert<std::string>();
                    user.password = rs[4].convert<std::string>();
                    user.email = rs[5].convert<std::string>();
                    user.title = rs[6].convert<std::string>();
                    result.push_back(user);
                    more = rs.moveNext();
                }
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
        if (result.empty())
        {
            return {};
        }
        return result;
    }

    std::vector<User> User::find_by_first_last_name(std::string &first_name, std::string &last_name)
    {
        std::vector<User> result;
        first_name += "%";
        last_name += "%";
        for (const auto& shard : database::Database::get_all_sharding_hints())
        {
            try
            {
                Poco::Data::Session session = database::Database::get_instance().create_database_session();
                Statement select(session);
                select << "select id, first_name, last_name, login, password, email, title from user where first_name like ? and last_name like ?;" + shard,
                        use(first_name),
                        use(last_name),
                        now;

                select.execute();
                Poco::Data::RecordSet rs(select);
                bool more =rs.moveFirst();
                while (more)
                {
                    User user;
                    user.user_uuid = rs[0].convert<std::string>();
                    user.first_name = rs[1].convert<std::string>();
                    user.last_name = rs[2].convert<std::string>();
                    user.login = rs[3].convert<std::string>();
                    user.password = rs[4].convert<std::string>();
                    user.email = rs[5].convert<std::string>();
                    user.title = rs[6].convert<std::string>();
                    result.push_back(user);
                    more = rs.moveNext();
                }
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
            catch(std::exception &e)
            {
                std::cout << "Database statement exception:" << e.what() << std::endl;
                throw;
            }
        }
        if (result.empty())
        {
            return {};
        }
        return result;
    }

    void User::save_to_db()
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            if (user_uuid.empty())
            {
                std::string uuid = boost::uuids::to_string(boost::uuids::random_generator()());
                user_uuid = uuid;
            }
            std::string shard = database::Database::get_sharding_hint(get_hash(user_uuid));
            Poco::Data::Statement insert(session);
            insert << "insert into user (id,first_name, last_name, login, password, email, title) values(?, ?, ?, ?, ?, ?, ?);"+shard,
                use(user_uuid),
                use(first_name),
                use(last_name),
                use(login),
                use(password),
                use(email),
                use(title),
                range(0,1);
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT @last_user_uuid;"+shard,
                into(user_uuid),
                range(0, 1); 

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "Inserted new user with id = " << user_uuid << std::endl;
            return;
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
        catch(std::exception &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
    }
    void User::send_to_queue() {
        Config &config = Config::get_instanse();
        static cppkafka::Configuration config_kafka = {
            {"metadata.broker.list",config.get_queue_host()},
            {"acks", "all"}
        };

        static cppkafka::Producer producer(config_kafka);
        static std::mutex mtx;
        static int message_key{0};
        using Hdr = cppkafka::MessageBuilder::HeaderType;

        std::lock_guard<std::mutex> lock(mtx);
        std::stringstream ss;
        Poco::JSON::Stringifier::stringify(to_json(), ss);
        std::string message = ss.str();
        std::cout << message;
        bool not_sent = true;

        cppkafka::MessageBuilder builder(config.get_queue_topic());
        std::string mk = std::to_string(++message_key);
        builder.key(mk);                                       // set some key
        builder.header(Hdr{"producer_type", "author writer"}); // set some custom header
        builder.payload(message);                              // set message

        while (not_sent) {
            try {
                producer.produce(builder);
                not_sent = false;
            }
                catch (...) {
            }
        }
    }

    void User::save_to_cache(std::string &key)
    {
        std::stringstream stream;
        Poco::JSON::Stringifier::stringify(to_json(), stream);
        std::string json = stream.str();
        database::Cache::get_instance().put(key, json);
    }
    std::string User::get_user_uuid() const
    {
        return user_uuid;
    }

    std::string User::get_first_name() const
    {
        return first_name;
    }

    std::string User::get_last_name() const
    {
        return last_name;
    }

    std::string User::get_login() const
    {
        return login;
    }

    std::string User::get_password() const
    {
        return password;
    }

    std::string User::get_email() const
    {
        return email;
    }

    std::string User::get_title() const
    {
        return title;
    }

    void User::set_first_name(std::string f_n)
    {
        first_name = f_n;
    }

    void User::set_last_name(std::string l_n)
    {
        last_name = l_n;
    }

    void User::set_login(std::string log)
    {
        login = log;
    }

    void User::set_password(std::string pswrd)
    {
        password = pswrd;
    }

    void User::set_email(std::string mail)
    {
        email = mail;
    }

    void User::set_title(std::string t_l)
    {
        title = t_l;
    }
    void User::set_user_uuid(std::string id)
    {
        user_uuid = id;
    }
}