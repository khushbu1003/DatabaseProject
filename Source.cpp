
//***************************************************************************
//*      Name: Khushbu Shah                                                 *
//*      Project 3 CSC 30500                                                *
//*      DataBase Management                                                *
//*      Date : 12 -10 - 2020                                               *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//*                                                                         *
//***************************************************************************

#define _CRT_SECURE_NO_WARNINGS
//preprocessor Directives
#include <iostream>
#include <string>

#include <mysql/jdbc.h> 

#include <conio.h>

using namespace std;

// reads in a password without echoing it to the screen
string myget_passwd()
{
	string passwd;

	for (;;)
	{
		char ch = 0;
		while (ch == 0)   // Ugh. Microsoft not following standards on getch, so need to keep reading until a new char is typed. 
			ch = _getch();           // get char WITHIOUT echo! (should really be getch(), but MS is not following standrads again.)
		if (ch == 13 || ch == 10) // if done ...
			break;           //  stop reading chars!
		cout << '*';  // dump * to screen
		passwd += ch;   // addd char to password
	}
	cin.sync(); // flush anything else in the buffer (remaining newline)
	cout << endl;  // simulate the enter that the user pressed

	return passwd;
}
//***********************MAIN FUNCTION************************************ 
int main()
{


	// strings for mysql hostname, userid, ...
	string db_host;
	string db_user;
	string db_password;
	string db_name;

	// set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; cout.flush();
	//mysql_init(&mysql);
	sql::Driver* driver = sql::mysql::get_driver_instance();
	cout << "Done!" << endl;

	// get user credentials and mysql server info
	cout << "Enter MySQL database hostname (or IP adress):";
	cin >> db_host;

	cout << "Enter MySQL database username:";
	cin >> db_user;

	cout << "Enter MySQL database password:";
	db_password = myget_passwd();

	// could also prompt for this, if desired
	//db_name = db_user;
	cout << "Enter a MySQL Database Name: ";
	cin >> db_name;       //same as db_user in my case


	try
	{
		// go out and connect to the mysql server
		cout << "Connecting to remote DB ..."; cout.flush();
		sql::Connection* conn = driver->connect(db_host, db_user, db_password);
		conn->setSchema(db_name);

		//conn = mysql_real_connect(&mysql,
			//db_host.c_str(), db_user.c_str(), db_password.c_str(), db_user.c_str(),
			//0, 0, 0); // last three are usually 0's


		cout << "DB connection established" << endl;

		//output statement before creating a table
		cout << "Creating table if need to be.." << endl;


		sql::Statement* stmt = conn->createStatement();
		sql::ResultSet* res;
		int TableStatus;

		//create table for the recipie
		string RecipieTable = "create table if not exists Recipie_T(RecipieName varchar(200) NOT NULL, Ingredient varchar(300) NOT NULL,"
			"Quantity int, primary key (RecipieName, Ingredient));";
		//sending table to the server
		TableStatus = stmt->execute(RecipieTable.c_str());

		//create table for the Inventory
		string InventoryTable = "create table if not exists Inventory_T(Ingredient varchar(300) NOT NULL, Quantity int );";
    	"CONSTRAINT fk_ingredient FOREIGN KEY(Ingredient) REFERENCES Recipie_T(Ingredient)  ON DELETE CASCADE ON UPDATE CASCADE);";
		//sending table to the server
		TableStatus = stmt->execute(InventoryTable.c_str());


		cout << "Done Creating" << endl;
		//you need to create a table that your PROGRAM NEEDS

		// now, send mysql our query ... and store the results	
		cout << "Sending query ..."; cout.flush();
		//stmt = conn->createStatement();
		//res = stmt->executeQuery("select * from Recipie_T");
		//res = stmt->executeQuery("select * from Inventory_T");



		cout << "Done" << endl;

		
		//declaring variable 
		int command;          //command variable
		int InsertStatus = 0;      //insertstatus initialization
		string RecipieName;         //name of the recipie variable
		string Ingredients;         //name of the Ingredients
		int Quantity;           //number of quantity
		//prompt users to choose one of the option
		cout << "1) Create a recipe or add ingredient to a recipe" << endl;
		cout << "2)List a recipe’s ingredients " << endl;
		cout << "3)Buy all recipe ingredients from the store" << endl;
		cout << "4)Add ingredients to the store" << endl;
			cout<<"5)Quit" << endl;
         //enter choice now
		cout << ">>>";
		do {
			// prompt for command

			cin >> command;

			if (command == 1)                  //is user choses 1
			{
				cout << "Enter a recipie and add ingredients and quantities to a recipie " << endl;

				cin >> RecipieName >> Ingredients >> Quantity;   //input declared variables

				
				cout << RecipieName << " " << Ingredients << " " << Quantity << endl;   //output declared variables

				//sql statement that will insert recipie, ingredients and quantity to the recipie table
				string InsertValue = "INSERT INTO Recipie_T (RecipieName, Ingredient, Quantity) VALUES(\"";
				InsertValue += RecipieName + "\", \"";
				InsertValue += Ingredients + "\", \"";
				InsertValue += to_string(Quantity) + "\");";

				cout << InsertValue << endl;

			
				stmt->execute(InsertValue);     //sending values to the table



			}


			else if (command == 2)
			{
				cout << "listing all recipie's ingredients" << endl;
				//When selected, this option should:
				//–Prompt the user to enter a recipe name.
				//–List the ingredients and required quantities for that recipe name.

				cout << "Enter a Recipe Name: " << endl;
				cin >> RecipieName;   //user input these var
				
				cout << "Sending Query ... " << endl; cout.flush();
				sql::Statement* stmt = conn->createStatement();
				

				//creating a sql statement that will compare userinput recipie to the Recipie table recipie
				sql::ResultSet* res = stmt->executeQuery("SELECT* FROM Recipie_T \ "
                                                           " WHERE Recipie_T.RecipieName =  RecipieName");
				/*string ListQuery = "SELECT Ingredient,Quantity FROM Recipie_T "
					"WHERE (RecipieName =   \"";
					              ListQuery += RecipieName + "\" );";
				*/
				
					//to check
				while (res->next())
				{
					cout << res->getString("Ingredient") << " " << res->getInt("Quantity") << " " << endl;

				}



			}
			else if (command == 3)           //if user chooses option 3 meaning they would like to purchase ingredients for the recipie
			{
				//prompt user to Enter the name of the recipie
				cout << "Buy all recipe ingredients from the store." << endl;
				cout << "Now Enter a recipie name" << endl;
				cin >> RecipieName;   

				//create a statement that  tells us that it is ready to start the transaction after selecting recipies
				string Transaction = "Start Transaction";
				cout << Transaction << endl;
				stmt->execute(Transaction);  

				//find all objects count
              string checkList1 = "SELECT Count(*) FROM Recipie_T WHERE RecipieName = \"Recipiename\";";
			  stmt->execute(checkList1);

			  //find all objects in both Inventory and recipie table
			   checkList1 = "SELECT Count(*) FROM Recipie_T,Inventory_T  WHERE RecipieName = \"Recipiename\" AND Recipie_T.Ingredient = Inventory_T.Ingredient;";
			  stmt->executeQuery(checkList1);


			  checkList1 = "SELECT Count(*) FROM Recipie_T,Inventory_T  WHERE RecipieName = \"Recipiename\" AND Recipie_T.Ingredient = Inventory_T.Ingredient AND Recipie_T.Quantity > Inventory_T.Quantity;";
			  stmt->executeQuery(checkList1);
			  
			  //creating a sql statement that will update a query from Inventory and recipie table
				//then set the inventory quantity to the final quantity which will be subtracted from the recipie table
				//and finally set both table's ingredients equal to each other to get the right ingredients.
			 string queryUpdate = "Update Inventory_T, Recipie_T SET Inventory_T.Quantity = Inventory_T.Quantity - Recipie_T.Quantity WHERE Inventory_T.Ingredient = Recipie_T.Ingredient;";
				/*string buyIngredients = "UPDATE Inventory_T, "
					"SET Inventory_T.Quantity= ' " + to_string(Quantity) + "' "
					"WHERE (Recipie_T.RecipieName= \"";
					buyIngredients +=  RecipieName + "\" \"";
					"AND Inventory_T.Ingredient = Recipie_T.Ingredient  \");";*/

				//"AND Inventory_T.Ingredient = Recipie_T.Ingredient  \");";
				//if so, check the output
				cout << queryUpdate << endl;

				 res = stmt->executeQuery(queryUpdate);     //sending statement to the transaction

				//again count, this will make impact on inventory checking status it will check if it has 0 or none
				string checkInventory = "SELECT count(Inventory_T.Quantity) AS object1 FROM Inventory_T WHERE Inventory_T.Quantity<0;";

				//to check the output
				cout << checkInventory << endl;

				 //send the inventory result back to the query 
				 res = stmt->executeQuery(checkInventory);     //sending values to the table

				 //if the result is not 0 
				if  (res!= 0)
				{
					//tranaction will rollback
					Transaction = "ROLLBACK;";
					//send that statement to the sql Query
					res = stmt->executeQuery(Transaction);
					//output the result for users
					cout << "Sorry, ingredients are not available in the inventory" << endl;
					
				}
				else
				{
					//make commit statement, if the trasaction is successful
					Transaction = "COMMIT;";
					 res = stmt->executeQuery(Transaction);     //sending statement to the table query
					cout << "There you go, you have successfully purchased the ingredients for: " << RecipieName << endl;
				}


			}

			else if (command == 4)        //if user chooses 4
			{
				//Prompt the user to enter an ingredient name and a quantity.
				//The corresponding ingredient in the inventory should have this quantity added to it.
					//A new ingredient can be added to the store using this option.

					//prompt user to enter both ingredients and quantity
				cout << "Please enter An ingredient name and a quantity: " << endl;
				//now add both value with space between them
				cout << ">>>";
				//reading in value
				cin >> Ingredients >> Quantity;

				//make a sql statement that will add both ingredients, and quanity to the table
				string InsertValue = "INSERT INTO Inventory_T(Ingredient,Quantity) VALUES(\"";
				                     InsertValue += Ingredients + "\", \"";
									 InsertValue += to_string(Quantity) + "\" )";
									 InsertValue += "ON DUPLICATE KEY UPDATE Quantity = \"" + to_string(Quantity) + "\";";
				                         
				//output the result	                 
				cout << InsertValue << endl;
				  
				stmt->execute(InsertValue);     //sending values to the table
				cout << "entry was updated:" << endl;
				

					
			}
			
			return 0;
		} while (command != 5);


		if (command == 5)                    //if user chooses 5
		{
			cout << "You have a great day" << endl;

			exit;                 //call exit 

		}
	}

	catch (sql::SQLException sqle)
	{
		cout << "Exception in SQL: " << sqle.what() << endl;
	}


}


