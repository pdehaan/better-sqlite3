#ifndef BETTER_SQLITE3_BINDER_H
#define BETTER_SQLITE3_BINDER_H

#include <sqlite3.h>
#include <nan.h>
class Query;

class Binder {
	public:
		Binder(sqlite3_stmt*, sqlite3_destructor_type);
		~Binder();
		virtual void Bind(Nan::NAN_METHOD_ARGS_TYPE, int, Query*);
		const char* GetError();
		
	protected:
		virtual int NextAnonIndex();
		void SetBindingError(int);
		void BindNumber(v8::Local<v8::Number>, int = 0);
		void BindString(v8::Local<v8::String>, int = 0);
		void BindBuffer(v8::Local<v8::Object>, int = 0);
		void BindNull(int = 0);
		void BindValue(v8::Local<v8::Value>, int = 0);
		
		int BindArray(v8::Local<v8::Array>);
		int BindArrayLike(v8::Local<v8::Object>, unsigned int);
		virtual int BindObject(v8::Local<v8::Object>, v8::Local<v8::Object>); // This should only be invoked once
		
		double GetArrayLikeLength(v8::Local<v8::Object>);
		static bool IsPlainObject(v8::Local<v8::Object>);
		
		sqlite3_stmt* handle;
		int param_count;
		
		int anon_index; // This value should only be used by NextAnonIndex()
		const char* error;
		char* error_extra;
		const char* error_full;
		
		sqlite3_destructor_type bind_type;
};

#endif