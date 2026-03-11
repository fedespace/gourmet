CREATE TABLE users
(    
   id bigserial PRIMARY KEY,    
   name varchar(255) NOT NULL,    
   email varchar(255) UNIQUE NOT NULL, 
   pwd_hash char(60) NOT NULL,    
   created_at timestamp default NOW()  
);