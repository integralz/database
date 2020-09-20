#include <stdio.h>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "wnstjr12";
const char* db = "project2";

int main(void) {
	FILE *fp, *cp;
	char value[100], value1[100], value2[100];
	char a[600], mid1[50], mid2[50], mid3[50];
	const char *query;
	int b, state;
	int che;
	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		const char * query1 = "select * from student";
		state = 0;

		state = mysql_query(connection, query1);
		if (state == 0)
		{
			sql_result = mysql_store_result(connection);
			while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				printf("%s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
			}
			mysql_free_result(sql_result);
		}

		fp = fopen("20141214_1.txt", "r");
		while (!feof(fp)) {
			fgets(a, 600, fp);
			state = mysql_query(connection, a);
			if (state != 0)
			{
				printf("error : %s \n", mysql_error(&conn));

			}
		}
	}

	while (1) {
		printf("------- SELECT QUERY TYPES -------\n\n");
		printf("\t1. TYPE I\n");
		printf("\t2. TYPE II\n");
		printf("\t3. TYPE III\n");
		printf("\t4. TYPE IV\n");
		printf("\t5. TYPE V\n");
		printf("\t0. QUIT\n");
		printf("----------------------------------\n");

		scanf("%s", &value);
		if (strcmp(value, "1") == 0) {
			printf("---- TYPE 1 ----\n\n");
			while (1) {
				printf("Input the number of truck : ");
				che = 0;
				scanf("%s", &value);
				if (strcmp(value, "0") == 0) {
					printf("back to menu\n");
					che = 2;
					break;
				}

				const char *query1 = "select * from transportation";
				state = 0;

				state = mysql_query(connection, query1);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						if (strcmp(sql_row[0], "truck") == 0 && strcmp(sql_row[1], value) == 0 && strcmp(sql_row[6], "destroyed") == 0) {
							che = 1;
							strcpy(mid1, sql_row[0]);
							strcpy(mid2, sql_row[1]);
							strcpy(mid3, sql_row[2]);
							break;
						}
					}
					mysql_free_result(sql_result);
					if (che == 0) {
						printf("Truck %s is not destroyed\n\n", value);
					}
					else break;
				}
			}
			if (che == 2) continue;

			while (1) {
				printf("------- Subtypes in TYPE I -------\n");
				printf("\t1. TYPE I-1.\n");
				printf("\t2. TYPE I-2.\n");
				printf("\t3. TYPE I-3.\n");
				printf("----------------------------------\n");

				scanf("%s", &value);
				if (strcmp(value, "1") == 0) {
					printf("---- TYPE I-1 ----\n\n");
					printf("** Find all customers who had a package on the truck at the time of the crash **\n");
					char query2[600] = "select c_name from transportation, carry, package, customer where carry.p_id = package.p_id and package.c_id = customer.c_id and transportation.t_type = carry.t_type and transportation.number = carry.number and transportation.t_start_time = carry.t_start_time and transportation.status = 'destroyed' and transportation.number = '";
					strcat(query2, mid2);
					strcat(query2, "';");
					state = 0;

					state = mysql_query(connection, query2);
					if (state == 0)
					{
						printf("Customer name : ");
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s ", sql_row[0]);
						}
						mysql_free_result(sql_result);
						printf("\n");
					}
				}
				else if (strcmp(value, "2") == 0) {
					printf("---- TYPE I-2 ----\n\n");
					printf("** Find all recipients who had a package on that truck at the time of the crash **\n");
					char query3[600] = "select r_name from transportation, carry, package, recipient where carry.p_id = package.p_id and package.r_id = recipient.r_id and transportation.t_type = carry.t_type and transportation.number = carry.number and transportation.t_start_time = carry.t_start_time and transportation.status = 'destroyed' and transportation.number = '";
					strcat(query3, mid2);
					strcat(query3, "';");
					state = 0;

					state = mysql_query(connection, query3);
					if (state == 0)
					{
						printf("Recipient name : ");
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s ", sql_row[0]);
						}
						mysql_free_result(sql_result);
						printf("\n");
					}
				}
				else if (strcmp(value, "3") == 0) {
					printf("---- TYPE I-3 ----\n\n");
					printf("** Find the last successful delivery by that truck prior to the crash **\n");

					char query3[800] = "select t_type, number, t_start_time, t_end_time, start_point, end_point from transportation where t_start_time >= all(select t_start_time from transportation where status != 'destroyed' and number = '";
					strcat(query3, mid2);
					strcat(query3, "')");
					strcat(query3, " and number = '");
					strcat(query3, mid2);
					strcat(query3, "' and status != 'destroyed';");
					state = 0;

					state = mysql_query(connection, query3);
					if (state == 0)
					{
						printf("last success : ");
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%s %s %s %s %s %s", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
						}
						mysql_free_result(sql_result);
						printf("\n");
					}
				}
				else if (strcmp(value, "0") == 0) {
					printf("bye\n");
					break;
				}

				else {
					printf("wrong order! back to select menu\n");
				}
			}
		}
		else if (strcmp(value, "2") == 0) {
			printf("---- TYPE II ----\n\n");
			while (1) {
				printf("** Find the customer who has shipped the most packages in certain year **\n");
				scanf("%s", &value);
				if (strcmp(value, "0") == 0) {
					printf("bye\n");
					break;
				}
				else {
					char query4[800] = "select c_name, count(c_name) as coun from bill, customer where bill.c_id = customer.c_id and year(time) = '";
					strcat(query4, value);
					strcat(query4, "' group by bill.c_id order by coun desc;");
					
					state = 0;

					state = mysql_query(connection, query4);
					if (state == 0)
					{
						printf("Customer name : ");
						sql_result = mysql_store_result(connection);
						if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {

							printf("%s\n", sql_row[0]);
						}
						mysql_free_result(sql_result);
						printf("\n");
					}
				}
			}
		}
		else if (strcmp(value, "3") == 0) {
			printf("---- TYPE III ----\n\n");
			while (1) {
				printf("** Find the customer who has spent the most money on shipping in the past certain year **\n");
				scanf("%s", &value);
				if (strcmp(value, "0") == 0) {
					printf("bye\n");
					break;
				}
				else {
					char query5[800] = "select c_name, sum(amount) as coun from bill, customer where bill.c_id = customer.c_id and year(time) = '";
					strcat(query5, value);
					strcat(query5, "' group by bill.c_id order by coun desc;");

					state = 0;

					state = mysql_query(connection, query5);
					if (state == 0)
					{
						printf("Customer name : ");
						sql_result = mysql_store_result(connection);
						if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
							printf("%s\n", sql_row[0]);
						}

						mysql_free_result(sql_result);
						printf("\n");
					}
				}
			}
		}
		else if (strcmp(value, "4") == 0) {
			printf("---- TYPE IV ----\n\n");
			printf("** Find those packages that were not delivered within the promised time **\n");
			char query6[1000] = "select package.p_id from transportation, carry, package, bill_package, bill, recipient, recipient_address where transportation.t_type = carry.t_type and transportation.number = carry.number and transportation.t_start_time = carry.t_start_time and package.p_id = carry.p_id and package.p_id = bill_package.p_id and bill_package.b_id = bill.b_id and recipient.r_id = package.r_id and recipient_address.r_name = recipient.r_name and  bill.promised_time < transportation.t_end_time and transportation.end_point = recipient_address.r_address;";
			state = 0;

			state = mysql_query(connection, query6);
			if (state == 0)
			{
				printf("package id : ");
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%s ", sql_row[0]);
				}
				mysql_free_result(sql_result);
				printf("\n");
			}
		}
		else if (strcmp(value, "5") == 0) {
			printf("---- TYPE V ----\n\n");
			printf("** Generate the bill for each customer for the past certain month.Consider creating several types of bills **\n");
			printf("Customer name : ");
			scanf("%s", &value);
			printf("Which year(YYYY)? ");
			scanf("%s", &value1);
			printf("Which month(mm)?");
			scanf("%s", &value2);
			printf("Generating Bill....\n");
			
			char query8[600] = "select customer.c_name, c_address, sum(amount) as coun from bill, customer, custom_address where custom_address.c_name = customer.c_name and bill.c_id = customer.c_id and year(time) = '";
			strcat(query8, value1);
			strcat(query8, "' and month(time) = '");
			strcat(query8, value2);
			strcat(query8, "'and  customer.c_name = '");
			strcat(query8, value);
			strcat(query8, "';");
			printf("Customer address Amount\n");

			state = 0;

			state = mysql_query(connection, query8);
			if (state == 0)
			{
				sql_result = mysql_store_result(connection);
				if ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
					printf("%s \t %s \t %s", sql_row[0], sql_row[1], sql_row[2]);
				}

				mysql_free_result(sql_result);
				printf("\n");
			}
			printf("| Package Number | Item | Amount | Serivce Type | Payment Type | Timeliness of Delivery |\n");
			char query9[1000] = "select bill_package.p_id, package.p_name, amount, p_type, b_type, t_end_time as coun from bill, customer, custom_address, bill_package, package, package_characterisitc, carry, transportation where package.p_name = package_characterisitc.p_name and package.p_id = bill_package.p_id and bill_package.b_id = bill.b_id and custom_address.c_name = customer.c_name and bill.c_id = customer.c_id and year(time) = '";
			strcat(query9, value1);
			strcat(query9, "' and month(time) = '");
			strcat(query9, value2); 
			strcat(query9, "' and customer.c_name = '");
			strcat(query9, value); 
			strcat(query9,"'and transportation.t_type = carry.t_type and transportation.number = carry.number and transportation.t_start_time = carry.t_start_time and package.p_id = carry.p_id;");
			state = 0;

			state = mysql_query(connection, query9);
			if (state == 0)
			{
			
				sql_result = mysql_store_result(connection);
				while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
				{
					printf("%s %s %s %s %s %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5]);
				}
				mysql_free_result(sql_result);
				printf("\n");
			}
	}
		else if (strcmp(value, "0") == 0) {
			printf("bye\n");
			break;
		}
		else {
			printf("wrong order!\n");
		}
	}

		mysql_close(connection);
		return 0;
}





/*

	printf("------- SELECT QUERY TYPES -------\n\n");
	printf("\t1. TYPE I\n");
	printf("\t2. TYPE II\n");
	printf("\t3. TYPE III\n");
	printf("\t4. TYPE IV\n");
	printf("\t5. TYPE V\n");
	printf("\t0. QUIT\n");
	//printf("----------------------------------\n");
	printf("\n\n");
	printf("---- TYPE II ----\n\n");
	printf("** Find the customer who has shipped the most packages in certain year**\n");
	printf(" Which Year? : 2018\n");
	printf(" Customer Name : Kim Yongdam\n");

	return 0;

}
*/