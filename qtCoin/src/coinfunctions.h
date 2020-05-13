#ifndef COINFUNCTIONS_H
#define COINFUNCTIONS_H




//void MainWindow::DeleteColFromTable(QString DbName, QString TableName, QString ColName){ //https://stackoverflow.com/questions/8442147/how-to-delete-or-add-column-in-sqlite
//    SQLiteDatabase db = openOrCreateDatabase(""+DbName+"", Context.MODE_PRIVATE, null);
//    db.execSQL("CREATE TABLE IF NOT EXISTS "+TableName+"(1x00dff);");
//    Cursor c = db.rawQuery("PRAGMA table_info("+TableName+")", null);
//    if (c.getCount() == 0) {

//    } else {
//        String columns1 = "";
//        String columns2 = "";
//        while (c.moveToNext()) {
//            if (c.getString(1).equals(ColName)) {
//            } else {
//                columns1 = columns1 + ", " + c.getString(1) + " " + c.getString(2);
//                columns2 = columns2 + ", " + c.getString(1);
//            }
//            if (c.isLast()) {
//                db.execSQL("CREATE TABLE IF NOT EXISTS DataBackup (" + columns1 + ");");
//                db.execSQL("INSERT INTO DataBackup SELECT " + columns2 + " FROM "+TableName+";");
//                db.execSQL("DROP TABLE "+TableName+"");
//                db.execSQL("ALTER TABLE DataBackup RENAME TO "+TableName+";");
//            }
//        }
//    }
//}




#endif // COINFUNCTIONS_H
