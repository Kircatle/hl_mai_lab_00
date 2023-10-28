#include "user.h"
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
// create table if not exists user
// (
//     id varchar(36) default uuid() primary key,
//     login text not null,
//     password text not null,
//     first_name text not null,
//     last_name text not null,
//     email text,
//     title text
// );
    void User::init()
    {
        try
        {

            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
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
                            << ");",
                            now;
                std::cout << create_stmt.toString() << std::endl;
                create_trigger 
                            << "CREATE TRIGGER IF NOT EXISTS last_uuiduser "
                            << "AFTER INSERT ON user "
                            << "FOR EACH ROW "
                            << "SET @last_user_uuid = NEW.id;",
                            now;
                std::cout << create_trigger.toString() << std::endl;
            create_index_user_first_name_index 
                            << "create index if not exists user_first_name_index on user(first_name);",
                            now;
            std::cout << create_index_user_first_name_index.toString() << std::endl;
            create_index_user_last_name_index
                            << "create index if not exists user_last_name_index on user(last_name);",
                            now;
            std::cout << create_index_user_last_name_index.toString() << std::endl;
            create_index_user_email_index
                            << "create index if not exists user_email_index on user(email);",
                            now;
            std::cout << create_index_user_email_index.toString() << std::endl;
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

    User User::from_json(const std::string &json)
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
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, login, password, email, title from user where id=?",
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
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, login, password, email, title from user where login=?",
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
        return {};
    }

    std::optional<User> User::auth_by_login(std::string &login, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, login, password, email, title from user where login=? and password=?",
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
        return {};
    }
    
   
    std::optional<User> User::auth_by_email(std::string &email, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, login, password, email, title from user where email=? and password=?",
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
        return {};
    }

    std::optional<User> User::find_by_login(std::string &login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement select(session);
            User user;
            login += "%";
            select << "select id, first_name, last_name, login, password, email, title from user where login like ?",
                    into(user.user_uuid),
                    into(user.first_name),
                    into(user.last_name),
                    into(user.login),
                    into(user.password),
                    into(user.email),
                    into(user.title),
                    use(login),
                    range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return user;
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

    std::optional<User> User::find_by_email(std::string &email)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement select(session);
            User user;
            select << "select id, first_name, last_name, login, password, email, title from user where email like ?",
                    into(user.user_uuid),
                    into(user.first_name),
                    into(user.last_name),
                    into(user.login),
                    into(user.password),
                    into(user.email),
                    into(user.title),
                    use(email),
                    range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return user;
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

    std::vector<User> User::find_by_first_last_name(std::string &first_name, std::string &last_name)
    {
        try
        {
            std::vector<User> result;
            first_name += "%";
            last_name += "%";

            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement select(session);
            select << "select id, first_name, last_name, login, password, email, title from user where first_name like ? and last_name like ?",
                    use(first_name),
                    use(last_name),
                    now;

            select.execute();
            Poco::Data::RecordSet rs(select);
            bool more = true;
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

    void User::save_to_db()
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement insert(session);
            insert << "insert into user (first_name, last_name, login, password, email, title) values(?, ?, ?, ?, ?, ?);",
                use(first_name),
                use(last_name),
                use(login),
                use(password),
                use(email),
                use(title),
                range(0,1);
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT @last_user_uuid;",
                into(user_uuid),
                range(0, 1); 

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "Inserted new user with id = " << user_uuid << std::endl;
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
}