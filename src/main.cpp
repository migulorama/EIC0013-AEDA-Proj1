#include <iostream>
#include <cstdlib>
#include <fstream>
#include <functional>

#include "gridmanager.h"
#include "loader.h"
#include "user.h"
#include "machine.h"
#include "bytebuffer.h"
#include "job.h"
#include "file.h"
#include "log.h"


struct MultipleWriter
{
	void Write(const std::string& val)
	{
		for (auto out: outs)
			*out << "GridManager -> " << val << std::endl;
	}
	
	void AddDestination(std::ostream* out) { outs.push_back(out); }

private:
	std::vector<std::ostream*> outs;
};

struct MyWriter : public MultipleWriter
{
	MyWriter()
	{
		file.open("file.txt");
		
		/*AddDestination(&std::cout);*/
		AddDestination(&file);

	}
private:
	std::ofstream file;
};

struct MyFormatter 
{
	std::string Format(const char* format, va_list args)
	{
		char res[1000];
		vsprintf(res, format, args);
		std::ostringstream out;
		out << res;
		return out.str();
	}
};

int main(int argc, char* argv[])
{
    Loader loader("test");

	/*MyLogger.Log("TODO: Everything %d", 10);
	
    typedef std::function<void(void)> T;
    bool executing = true;

	Menu<T> mainMenu("Main Menu");

    Menu<T>* AddUserMenu = new Menu<T>("Add User");

    AddUserMenu->addMenu('A', new MenuItem<T>("Add Academic User", [](){return;}));
    AddUserMenu->addMenu('E', new MenuItem<T>("Add Enterprise User", [](){return;}));

	mainMenu.addMenu('A', AddUserMenu);
    mainMenu.addMenu('E', new MenuItem<T>("Exit", [&executing](){executing=false;}));*/

   GridManager* gm = loader.Load();

    if (!gm)
    {
        gm = new GridManager();
        Machine* m = new Machine("Machine1", 2, 200, 200);
        User* u = new EnterpriseUser("User1", 200);
        Job* j = new Job("Job1", 100, 200, 200, 5);

        m->AddJob(j);

        gm->AddMachine(m);
        gm->AddUser(u);
    }
	
    


	system("pause");

    ByteBuffer bb(100);
    gm->Save(bb);

    File::Save("test", (char*)bb.Data(), bb.Size());

    system("PAUSE");

    // delete gm;
   
    return EXIT_SUCCESS;
}
