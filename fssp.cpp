#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <set>
#include <unordered_set>
using namespace std;

namespace sio{
	int open(const char *filename, char *buffer){
		FILE *f = fopen(filename,"rb");
		fseek(f,0,SEEK_END);
		int len = ftell(f);
		rewind(f);
		fread(buffer,len,1,f);
		fclose(f);
		return len;
	}
	void save(const char *filename, char *buffer, int length){
		FILE *f = fopen(filename,"wb");
		fwrite(buffer,length,1,f);
		fclose(f);
	}
}

long long int uks=0;

unordered_set<string> owned;
set<string> not_owned;

void search(string path){
	for(const auto & entry : filesystem::directory_iterator(path)){
		const char *pathc = entry.path().c_str();
		if(strcmp(pathc,"/boot")==0)continue;
		if(strcmp(pathc,"/dev")==0)continue;
		if(strcmp(pathc,"/efi")==0)continue;
		if(strcmp(pathc,"/lost+found")==0)continue;
		if(strcmp(pathc,"/mnt")==0)continue;
		if(strcmp(pathc,"/opt")==0)continue;
		if(strcmp(pathc,"/proc")==0)continue;
		if(strcmp(pathc,"/run")==0)continue;
		if(strcmp(pathc,"/sys")==0)continue;
		if(strcmp(pathc,"/tmp")==0)continue;
		if(strcmp(pathc,"/lib64")==0)continue;
		if(strcmp(pathc,"/lib")==0)continue;
		if(strcmp(pathc,"/sbin")==0)continue;
		if(strcmp(pathc,"/bin")==0)continue;
		if(strcmp(pathc,"/srv")==0)continue;
		if(strcmp(pathc,"/var/run")==0)continue;
		if(strcmp(pathc,"/var/shm")==0)continue;
		if(strcmp(pathc,"/var/tmp")==0)continue;
		if(strcmp(pathc,"/home/korsic/trenut")==0)continue;
		if(strcmp(pathc,"/home/korsic/chromium/cache")==0)continue;
		if(strcmp(pathc,"/home/korsic/.cache/pikaur")==0)continue;
		if(strcmp(pathc,"/home/korsic/.local/share/pikaur/aur_repos")==0)continue;
		/* ostaje samo:
		* etc
		* home
		* usr
		* var
		*/
		bool fu=false;
		if(owned.find(pathc)==owned.end()){
			fu=true;
			not_owned.insert(pathc);
		}
		error_code ec;
		string ts = entry.path().string();
		if(filesystem::is_symlink(ts,ec))
			continue;
		if(filesystem::is_regular_file(ts,ec)&&fu){
			long long int tus=filesystem::file_size(entry.path());
			uks+=tus;
		}
		if(filesystem::is_directory(ts,ec))
			search(ts);
	}
}

char lf[1000000000];

void fown(){
	string pp = "/var/lib/pacman/local";
	for(const auto & entry : filesystem::directory_iterator(pp)){
		error_code ec;
		if(!filesystem::is_directory(entry.path(),ec)) continue;
		string fp = entry.path().string() + "/files";
		int duf=sio::open(fp.c_str(),lf);
		int i1=0;
		for(int i2=0;i2<duf;++i2){
			if(lf[i2]=='\n'){
				lf[i2]=0;
				string ai(lf+i1,lf+i2);
				owned.insert("/" + ai);
				i1=i2+1;
			}
		}
	}
}

int main(int argc, char **argv){
	if(argc<2) return 1;
	fown();
	string pas(argv[1]);
	search(pas);
	printf("%.2f MiB not owned\n",uks/(1024*1024.0));
	/*
	char *u=lf;
	for(auto x:owned){
		const char *ey = x.c_str();
		strcpy(u,ey);
		u+=strlen(ey);
		*u='\n';
		++u;
	}
	sio::save("owned",lf,u-lf);
	u=lf;
	for(auto x:not_owned){
		const char *ey = x.c_str();
		strcpy(u,ey);
		u+=strlen(ey);
		*u='\n';
		++u;
	}
	sio::save("not_owned",lf,u-lf);
	*/
	return 0;
}
