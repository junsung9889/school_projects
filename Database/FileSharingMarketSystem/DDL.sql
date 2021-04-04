create table user
    (name   varchar(10) NOT NULL,
     address    varchar(10),
     account varchar(15),
     phone   varchar(15),
     birthday   date,
     access_history varchar(15),
     subscription_fee   int DEFAULT 50000,
     amount_due int DEFAULT 50000,
     date_joined    date,
     data_joined int DEFAULT 0,
     primary key (name)
    );

create table provider
    (name   varchar(10) NOT NULL,
     address    varchar(10),
     account varchar(15),
     phone   varchar(15),
     birthday   date,
     joining_fee   int DEFAULT 50000,
     due_you int DEFAULT 50000,
     stil_to_you  int DEFAULT 50000,
     to_provider int DEFAULT 0,
     data_joined    int DEFAULT 0,
     primary key (name)
    );

create table item
    (id varchar(20) NOT NULL,
     provider_name varchar(20) NOT NULL,
     name   varchar(20) NOT NULL,
     type   varchar(20) NOT NULL,
     size   int NOT NULL,
     author varchar(20),
     subject    varchar(20),
     machine    varchar(20),
     os varchar(20),
     language   varchar(20),
     local_storage  int DEFAULT 0,
     short_description varchar(50),
     address_retrieved  int DEFAULT 0,
     viewer    varchar(20),
     audio  varchar(20),
     last_updated   date,
     primary key (id,provider_name,name,type,size),
     foreign key (provider_name) references provider(name)
		on delete cascade
    );

create table download
    (user_name varchar(10) NOT NULL,
     item_id varchar(20) NOT NULL,
     provider_name varchar(10) NOT NULL,
     item_name varchar(20) NOT NULL,
     item_type varchar(20) NOT NULL,
     item_size int NOT NULL,
     primary key (user_name, provider_name, item_id, item_name, item_type, item_size),
     foreign key (user_name) references user(name)
		on delete cascade,
     foreign key (item_id,provider_name,item_name,item_type,item_size) references item(id,provider_name,name,type,size)
    );

delimiter //
create trigger update_user after insert on download
for each row
begin
    update user
    set access_history = new.item_id
    where name = new.user_name;

    update user
    set data_joined = data_joined + 1
    where name = new.user_name;
end //

create trigger update_provider after insert on download
for each row
begin
    declare amount int;
    update provider
    set data_joined = data_joined + 1
    where name = new.provider_name;

    set amount = (select local_storage from item where new.provider_name = provider_name);
    update provider
    set to_provider = amount * data_joined
    where name = new.provider_name;

    update provider
    set stil_to_you = due_you - to_provider
    where name = new.provider_name;
end //

create trigger update_local_storage before insert on item
for each row
begin
    set new.local_storage = new.size * 100;
end //

create trigger update_due_you after insert on item
for each row
begin
    update provider
    set due_you = due_you + new.local_storage
    where name = new.provider_name;
end //

create event bill
    on schedule every 1 month
    on completion preserve enable
    comment 'Bills updated'
    do
    begin
        update user
        set amount_due = amount_due + 50000;
        
        update provider
        set due_you = due_you + (to_provider/data_joined)
        where data_joined != 0;
    end //
delimiter ;