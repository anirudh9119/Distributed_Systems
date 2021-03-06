/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
#include <stdio.h>
#include <rpc/rpc.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <chrono>
#include <dlfcn.h>
#include "../Proto/MapReduce.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
 #include <mutex>
 #include <future> 
 #include <functional>


using namespace std;
using namespace std::chrono;

mutex barrier1;

#include "../header/tasktracker.h"

char* convertString(string s)
{
	char *ret;
	ret = new char[s.length()+1];
	strcpy(ret,s.c_str());
	return ret;
}


char **
mapper_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result="hi";
	//cout << "hi" << endl;
	string data(*argp);
	MapTaskInfo info;
	info.ParseFromString(data);
	int len = info.inputblocks_size();
	int numReduce = info.numreducetasks();
	string map_name = info.mapname();
	int job = info.jobid();
     void *lib = dlopen("./libwordcount.so", RTLD_LAZY);
      if (!lib) {
                cerr << "Cannot open library: map " << dlerror() << '\n';
                return &result;
        }

	for(int i=0;i<len;i++)
	{
			typedef void (*lib_function)(char *block,int numReduceTasks,int jobId);
			lib_function func = (void (*)(char *block,int numReduceTasks,int jobId))dlsym(lib,map_name.c_str() );
			const char *dlsym_error = dlerror();
        	if (dlsym_error) {
            cerr << "Cannot load symbol 'hello': " << dlsym_error <<'\n';
            dlclose(lib);
            return &result;
        }
        
        string s = to_string(info.inputblocks(i));
        char *block;
        block = new char[s.size()+1];
        strcpy(block,s.c_str());
        func(block,numReduce,job);

		cout << " block " << info.inputblocks(i) << endl;
	}
		
	dlclose(lib);
	/*
	 * insert server code here
	 */

	return &result;
}

char **
reducer_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result="hi";
	//cout << " Hellio " << endl;
	//barrier1.lock();
	ReducerTaskInfo info;
	//printf("%s")
	string data(*argp);
	cout << data << endl;
	info.ParseFromString(data);
	int jobId = info.jobid();
	int taskId = info.taskid();
	int reducerNumber = info.reducernumber();
	string reducerName = info.reducername();
	cout << " RecudenName : " << reducerName << " " << reducerNumber << endl;
	//barrier1.unlock();
	string output = info.outputfile();
	string ownIP =info.ownip();
	CLIENT *clnt;
	int len = info.mapoutputlocation_size();
	vector <string> reducedata;
	string filename = "mapOut_"+to_string(jobId)+"_"+to_string(reducerNumber);

	future< char** > futures;
	int k=0;			
	for(int i=0;i<len;i++)
	{
		string a = info.mapoutputlocation(i);
		if(a!=ownIP)
		{
			/*char *host = convertString(a);
			ReducerInputFileRequest Request;
			Request.set_filename(filename);
			string op;
			if(!Request.SerializeToString(&op))
			{
				cout << "Failed to write" <<endl;
				exit(0);
			}
			bool_t bres;
			
			char *arg = convertString(op);
			char* *result_1;
			#ifndef	DEBUG
			clnt = clnt_create (host, TASKTRACKER, TT, "tcp");
			if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
			}
			#endif	/* DEBUG */
			//cout << " call" << endl;
			//result_1 = reducerinput_1(&arg,clnt);
			/*futures = async(std::launch::async,reducerinput_1,&arg,clnt);
			#ifndef	DEBUG
			clnt_destroy (clnt);
			#endif	 /* DEBUG */
			//cout << " Taken from block " << i << endl;
		}
		else
		{
			std::ifstream file(filename.c_str());
			std::string str;
			while (std::getline(file, str))reducedata.push_back(str);
		
  		}

	}
/*	char** result_1;
	
	cout << " done till here k " << k << endl;
	for(int i=0;i<=k;i++) {
		if(futures.valid()){
		//cout <<  futures.get() << endl;
			result_1 = futures.get();
	}
	else cout << " Invialid " << endl;
	
	cout <<  i  << endl;
   	
	//	if(e.get()!=NULL)result_1 = e.get();
   	string data(*result_1);
	ReducerInputFileResponse Response;
	Response.ParseFromString(data);
	int len2 = Response.fileline_size();
	for(int j=0;j<len2;j++)
	{
		cout << Response.fileline(j) << endl;
		reducedata.push_back(Response.fileline(j));
	}
	

  }
  */
  cout << " done till here 2 " << endl;
	void *lib = dlopen("./libwordcount.so", RTLD_LAZY);
      if (!lib) {
                cerr << "Cannot open library: map " << dlerror() << '\n';
                return &result;
        }
    typedef void (*lib_function)(vector <string> data,int reducerNumber);
    cout << "-----" << reducerName << endl;
    if(reducerName.size()>0)
    {
    lib_function func = (void (*)(vector <string> data,int reducerNumber))dlsym(lib,reducerName.c_str() );
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
                cerr << "Cannot load symbol 'reedce': " << dlsym_error <<
                        '\n';
                dlclose(lib);
                return &result;
    }
    func(reducedata,reducerNumber);
	}
	cout << " call ended " << endl;



	/*
	 * insert server code here
	 */

	return &result;
}


char **
reducerinput_1_svc(char **argp, struct svc_req *rqstp)
{
	char * result="hi";
	cout << " Done " << endl;
	if(*argp!=NULL)
	{
		string data(*argp);
		ReducerInputFileRequest Request;
		Request.ParseFromString(data);
		string filename = Request.filename();
		cout << filename << " file " << endl;
		std::ifstream file(filename.c_str());
		std::string str;
		std::string file_contents="";
		ReducerInputFileResponse Response;
		
		while (std::getline(file, str))
		{
			Response.add_fileline(str);
			cout << " adding " << endl;
		
  		}  
  		string op;
		if(!Response.SerializeToString(&op))
		{
			cerr << "Failed to write" <<endl;
			exit(0);
		}
	
		result = convertString(op);

	}
	return &result;
}