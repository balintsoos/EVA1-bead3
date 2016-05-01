DROP DATABASE IF EXISTS videorentdb;
CREATE DATABASE videorentdb;
USE videorentdb;

CREATE TABLE member(
  id INTEGER PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(30) NOT NULL,
  address VARCHAR(40) NOT NULL,
  phone VARCHAR(20) NOT NULL
);

CREATE TABLE movie(
  id INTEGER PRIMARY KEY AUTO_INCREMENT,
  title VARCHAR(40),
  main_characters VARCHAR(50),
  director VARCHAR(40),
  distributor VARCHAR(40),
  release_date Date,
  available INTEGER NOT NULL
);

CREATE TABLE rent(
  rent_id INTEGER PRIMARY KEY AUTO_INCREMENT,
  member_id INTEGER,
  movie_id INTEGER,
  start_date Date,
  end_date Date,
  CONSTRAINT renttomember
    FOREIGN KEY (member_id)
    REFERENCES member (id),
  CONSTRAINT renttomovie
    FOREIGN KEY (movie_id)
    REFERENCES movie (id)
);

insert into member(name, address, phone) values('Balint Soos', 'Budapest', '+36 20 111 1111');
insert into member(name, address, phone) values('John Smith', 'Los Angeles', '(213) 974-3211');

insert into movie(title, main_characters, director, distributor, release_date, available)
  values('Star Wars', 'Mark Hamill, Harrison Ford, Carrie Fisher', 'George Lucas', '20th Century Fox', '1977-05-25', 1);

insert into movie(title, main_characters, director, distributor, release_date, available)
  values('The Prestige', 'Christian Bale, Hugh Jackman', 'Christopher Nolan', 'Warner Bros. Pictures', '2006-10-20', 1);
