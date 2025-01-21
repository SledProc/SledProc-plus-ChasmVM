

// //   this is not a top-level include; 
 //     it is designed to be inserted 
 //     into a class definition.

QString str_get_flag_union() const
{
 return QString::number(get_flag_union());
}

Flags_type get_flag_union() const
{
 return Flags;
}

void read_flag_union(Flags_type fls)
{
 Flags = fls;
}

void str_read_flag_union(QString str)
{
 read_flag_union(str.toULongLong());
}





