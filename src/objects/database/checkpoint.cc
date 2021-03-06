// .checkpoint([boolean force], Function callback) -> this

NAN_METHOD(Database::Checkpoint) {
	TRUTHINESS_OF_ARGUMENT(0, force);
	Database* db = Nan::ObjectWrap::Unwrap<Database>(info.This());
	if (db->in_each) {
		return Nan::ThrowTypeError("This database connection is busy executing a query.");
	}
	if (db->state != DB_READY) {
		return Nan::ThrowError("The database connection is not open.");
	}
	
	int total_frames;
	int checkpointed_frames;
	int status = sqlite3_wal_checkpoint_v2(
		db->db_handle,
		"main",
		force ? SQLITE_CHECKPOINT_RESTART : SQLITE_CHECKPOINT_PASSIVE,
		&total_frames,
		&checkpointed_frames
	);
	
	if (status != SQLITE_OK) {
		CONCAT2(message, "SQLite: ", sqlite3_errmsg(db->db_handle));
		return Nan::ThrowError(message);
	}
	
	if (checkpointed_frames < 0 || total_frames < 0) {
		info.GetReturnValue().Set(Nan::New<v8::Number>(0));
	} else if (total_frames == 0) {
		info.GetReturnValue().Set(Nan::New<v8::Number>(1));
	} else {
		info.GetReturnValue().Set(Nan::New<v8::Number>(
			(double)checkpointed_frames / (double)total_frames
		));
	}
}
