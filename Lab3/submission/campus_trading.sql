/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2026/4/24 18:44:39                           */
/*==============================================================*/


/*==============================================================*/
/* Table: admin                                                 */
/*==============================================================*/
create database campus_trading_platform;
use campus_trading_platform;

create table admin
(
   admin_id             int not null,
   admin_name           char(18) not null,
   admin_password       char(50) not null,
   primary key (admin_id)
);

/*==============================================================*/
/* Table: category                                              */
/*==============================================================*/
create table category
(
   category_id          int not null,
   category_name        char(20) not null,
   category_details     char(255),
   primary key (category_id)
);

/*==============================================================*/
/* Table: collect                                               */
/*==============================================================*/
create table collect
(
   user_id              int not null,
   goods_id             int not null,
   collect_time         datetime not null
);

/*==============================================================*/
/* Table: college                                               */
/*==============================================================*/
create table college
(
   college_id           int not null,
   college_name         char(30) not null,
   primary key (college_id)
);

/*==============================================================*/
/* Table: complaint                                             */
/*==============================================================*/
create table complaint
(
   complaint_id         int not null,
   user_id              int not null,
   order_id             int not null,
   admin_id             int,
   complaint_time       datetime not null,
   complaint_reason     char(255) not null,
   complaint_status     char(10) not null,
   complaint_result     char(50) not null,
   primary key (complaint_id)
);

/*==============================================================*/
/* Table: goods                                                 */
/*==============================================================*/
create table goods
(
   goods_id             int not null,
   category_id          int not null,
   user_id              int not null,
   order_id             int,
   goods_title          char(20),
   goods_details        char(255),
   goods_price          float(10) not null,
   goods_time           datetime not null,
   goods_status         char(10) not null,
   primary key (goods_id)
);

/*==============================================================*/
/* Table: "orderx"                                               */
/*==============================================================*/
create table orderx
(
   order_id             int not null,
   user_id              int not null,
   order_time           datetime not null,
   order_amount         float(10) not null,
   order_status         char(10) not null,
   primary key (order_id)
);

/*==============================================================*/
/* Table: punish                                                */
/*==============================================================*/
create table punish
(
   punish_id            int not null,
   admin_id             int not null,
   user_id              int not null,
   goods_id             int,
   punish_time          datetime not null,
   punish_reason        char(255) not null,
   punish_result        char(255) not null,
   primary key (punish_id)
);

/*==============================================================*/
/* Table: user                                                  */
/*==============================================================*/
create table user
(
   user_id              int not null,
   college_id           int not null,
   user_name            char(10) not null,
   phone_num            int not null,
   login_password       char(50) not null,
   account_status       char(10) not null,
   primary key (user_id)
);

alter table collect add constraint FK_goods_collect foreign key (goods_id)
      references goods (goods_id) on delete restrict on update restrict;

alter table collect add constraint FK_user_collect foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table complaint add constraint FK_admin_complaint foreign key (admin_id)
      references admin (admin_id) on delete restrict on update restrict;

alter table complaint add constraint FK_complaint_order foreign key (order_id)
      references orderx (order_id) on delete restrict on update restrict;

alter table complaint add constraint FK_user_complaint foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table goods add constraint FK_category_goods foreign key (category_id)
      references category (category_id) on delete restrict on update restrict;

alter table goods add constraint FK_order_goods foreign key (order_id)
      references orderx (order_id) on delete restrict on update restrict;

alter table goods add constraint FK_user_goods_publish foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table orderx add constraint FK_user_order foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table punish add constraint FK_admin_punish foreign key (admin_id)
      references admin (admin_id) on delete restrict on update restrict;

alter table punish add constraint FK_goods_punish foreign key (goods_id)
      references goods (goods_id) on delete restrict on update restrict;

alter table punish add constraint FK_user_punish foreign key (user_id)
      references user (user_id) on delete restrict on update restrict;

alter table user add constraint FK_college_user foreign key (college_id)
      references college (college_id) on delete restrict on update restrict;

