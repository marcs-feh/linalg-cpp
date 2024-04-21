#include "linalg.hpp"

#include <cstdio>
#include <vector>

struct Test {
	using Func = void (*)(Test&);
	int total = 0;
	int failed = 0;
	char const* title = nullptr;
	std::vector<Func> funcs;

	Test& use(Func f){
		funcs.push_back(f);
		return *this;
	}

	void ensure(bool predicate, char const* msg = ""){
		total += 1;
		if(!predicate){
			failed += 1;
			std::fprintf(stderr, "%s\n", msg);
		}
	}

	template<typename Err, typename Fn>
	void except(Fn&& f){
		total += 1;
		try {
			f();
		}
		catch(Err e){
			return;
		}
		failed += 1;
	}
	
	bool run(){
		for(auto f : funcs){
			f(*this);
		}

		report();
		return failed <= 0;
	}

	void report(){
		std::printf("[ %s ]: ", title);
		if(failed <= 0){
			std::printf("PASSED");
		}
		else {
			std::printf("FAILED");
		}
		std::printf(". ok in %d/%d\n", total - failed, total);
	}

	Test(char const* title)
		:title{title} {}

};

int main(){
	auto t = Test("Linear Algebra");
	t.run();
}

