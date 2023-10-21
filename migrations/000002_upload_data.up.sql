START TRANSACTION;
INSERT INTO `delivery`.`user` (`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("15d82ae6-1a47-473f-9423-fdf23e99164c","kircatle","qwerty","Kirill","Syroezhkin","kircatle@yandex.ru","he");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("c9e0aff7-9b9c-4a24-b40a-e6c7e45f8402 ","Kitty","qwerty1","Olga","Komarova","super_olya@yandex.ru","she");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("2e040d71-d9b7-4221-9bd5-3360c5e17a96","SuperStar223","qwerty3","Evgeniy","Lobanov","evg__1__1@yandex.ru","he");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("6ea19130-6c03-4b21-8f0f-e7f5d8119fb9","AlexSSSander","qwerty4","Alex","Burdonov","Pro100Sasha@yandex.ru","he");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("dab91bc4-cf02-44a1-a4fd-d936f3e59715","DarkLord","qwerty","Grigory","Surzhikov","grigoryHipe@yandex.ru","he");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("832035db-f5e6-46d6-bbe7-ca4ae2c4145d","SpiderKiss","qwerty","Daria","Kryilova","dasha2005_uncly@yandex.ru","she");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("670720e4-ed7c-48bc-a945-5fc3a87e21d3","object","qwerty","Maksim","Andreev","max_object@yandex.ru","he");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("8b353ea6-bf9b-47eb-b9b8-6a1003a92615","AntonFF","qwerty","Anton","Avramenko","anton_dev@yandex.ru","he");
INSERT INTO `delivery`.`user`(`id`,`login`,`password`,`first_name`,`last_name`,`email`,`title`)
VALUES ("fd378e59-87df-408e-85c8-46ff45c38b41","LadyBoy","qwerty","Evgenie","Uald","ladyBoy@yandex.ru","she");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("4c2fbec5-4cdf-4b96-aab9-18260277cf56", "15d82ae6-1a47-473f-9423-fdf23e99164c","Playstation 4",33,22,11,55,"with games");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("9c2ac23a-ee14-41de-a3cf-81dfe3bc0c1c", "8b353ea6-bf9b-47eb-b9b8-6a1003a92615","Mirror",33,22,11,55,"big mirror");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("d3aa5223-6a1c-42a3-9e74-39476f170ba7", "6ea19130-6c03-4b21-8f0f-e7f5d8119fb9","Black door",33,22,11,55,"wooden door");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("c44dd58a-4cd0-470b-95a7-b3cfb50d50de", "6ea19130-6c03-4b21-8f0f-e7f5d8119fb9","Crocodile",33,22,11,55,"so soft");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("65bef3b7-2a13-4533-8fd8-e6cc98f3ec0a", "dab91bc4-cf02-44a1-a4fd-d936f3e59715","keyboard",33,22,11,55,"mechanical");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("80bd01ff-c05b-458e-90b5-6a007e4472f8", "c9e0aff7-9b9c-4a24-b40a-e6c7e45f8402","Sugar",33,22,11,55,"sugar");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("9c222c22-032c-4704-9ee7-d3ff656e0f91", "dab91bc4-cf02-44a1-a4fd-d936f3e59715","Book",33,22,11,55,"dict");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("801a7aa5-8b89-4e88-9a0c-6f3c1304101f", "2e040d71-d9b7-4221-9bd5-3360c5e17a96","letter",33,22,11,55,"some letter");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("0f05f089-9421-4c0f-84fd-2eaeae6a5ebd", "2e040d71-d9b7-4221-9bd5-3360c5e17a96","cup",33,22,11,55,"broken");
INSERT INTO `delivery`.`package`(`id`,`user_id`,`title`,`width`,`height`,`length`,`weight`,`description`)
VALUES ("fa5d31db-5d90-4fea-8560-4107cc83fd70", "2e040d71-d9b7-4221-9bd5-3360c5e17a96","book",33,22,11,55,"with jokes");
INSERT INTO `delivery`.`delivery`(`package_id`,`sender_id`,`receiver_id`,`departure_date`,`start_point`,`destination`,`status`)
VALUES ("d3aa5223-6a1c-42a3-9e74-39476f170ba7","6ea19130-6c03-4b21-8f0f-e7f5d8119fb9","15d82ae6-1a47-473f-9423-fdf23e99164c",'2023-10-21 22:31:25','city Bronnitsy st. Moscowscaya h 99','Mowcow st. Leninskaya h 111','in transit');
INSERT INTO `delivery`.`delivery`(`package_id`,`sender_id`,`receiver_id`,`departure_date`,`start_point`,`destination`,`status`)
VALUES ("c44dd58a-4cd0-470b-95a7-b3cfb50d50de","6ea19130-6c03-4b21-8f0f-e7f5d8119fb9","15d82ae6-1a47-473f-9423-fdf23e99164c",'2023-10-20 18:31:25','city Omsk st. Moscowscaya h 99','Mowcow st. Leninskaya h 111','completed');
INSERT INTO `delivery`.`delivery`(`package_id`,`sender_id`,`receiver_id`,`departure_date`,`start_point`,`destination`,`status`)
VALUES ("4c2fbec5-4cdf-4b96-aab9-18260277cf56","15d82ae6-1a47-473f-9423-fdf23e99164c","15d82ae6-1a47-473f-9423-fdf23e99164c",'2023-10-19 15:31:25','city Evpatoria st. Moscowscaya h 99','Mowcow st. Leninskaya h 111','completed');
INSERT INTO `delivery`.`delivery`(`package_id`,`sender_id`,`receiver_id`,`departure_date`,`start_point`,`destination`,`status`)
VALUES ("9c2ac23a-ee14-41de-a3cf-81dfe3bc0c1c","8b353ea6-bf9b-47eb-b9b8-6a1003a92615","15d82ae6-1a47-473f-9423-fdf23e99164c",null,'city Anapa st. Moscowscaya h 99','Mowcow st. Leninskaya h 111','prepared');
INSERT INTO `delivery`.`delivery`(`package_id`,`sender_id`,`receiver_id`,`departure_date`,`start_point`,`destination`,`status`)
VALUES ("65bef3b7-2a13-4533-8fd8-e6cc98f3ec0a","dab91bc4-cf02-44a1-a4fd-d936f3e59715","15d82ae6-1a47-473f-9423-fdf23e99164c",'2023-10-15 11:31:25','city Mitishi st. Moscowscaya h 99','Mowcow st. Leninskaya h 111','in transit');
COMMIT;