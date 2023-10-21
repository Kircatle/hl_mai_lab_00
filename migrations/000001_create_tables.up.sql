START TRANSACTION;
create table if not exists delivery
(
    id varchar(36) default uuid() primary key,
    package_id varchar(36) not null,
    sender_id varchar(36) not null,
    receiver_id  varchar(36) not null,
    departure_date datetime,
    start_point text,
    destination text,
    status text
);
create index if not exists sender_id on delivery(sender_id);
create index if not exists receiver_id on delivery(receiver_id);


create table if not exists user
(
    id varchar(36) default uuid() primary key,
    login text not null,
    password text not null,
    first_name text not null,
    last_name text not null,
    email text,
    title text
);

create index if not exists user_first_name_index on user(first_name);
create index if not exists user_last_name_index on user(last_name);

create table if not exists package
(
    id varchar(36) default uuid() primary key,
    user_id varchar(36)      not null,
    title text    not null,
    width  double precision not null,
    height double precision not null,
    length double precision not null,
    weight double precision not null,
    description text
);

create index if not exists package_user on package(user_id);
commit;