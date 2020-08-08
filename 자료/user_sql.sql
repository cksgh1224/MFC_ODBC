select * from user;
select mid,mpw,mname from user;
select count(*) from user;

insert into user (mid,mpw,mname,mip,mnickname)
values ('cksgh1224','1234','윤찬호','192.168.77.100','호옹이');

insert into user (mid,mpw,mname,mip,mnickname)
values ('test','0000','테스터','127.0.0.1','관리자');

insert into user (mid,mpw,mname,mip,mnickname) 
values('123', '123', '123', '123', '123');

delete from user where mid='123';

update user set mpw='asd' where mid='123';


select * from user where mid='cksgh1224' and mpw='12345';
select * from user where mname='윤찬호' and mnickname='호옹이';
select count(*) from user where mid='cksgh1224';