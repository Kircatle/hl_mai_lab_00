START TRANSACTION;
drop index package_user on package;
drop table if exists package;
drop index user_last_name_index on user;
drop index user_first_name_index on user;
drop table user;
drop index sender_id on delivery;
drop index receiver_id on delivery;
drop table delivery;
commit;


