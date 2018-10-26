String json_maker(String *data_name, String *data_int){
    
    String return_string = String("[{");
    String Quotes = "'";
    int i=0;
    while(data_name[i]){
      return_string = String(return_string + "'" + data_name[i]+ "':");
      return_string = String(return_string + data_int[i]);
      if(data_name[i+1] != NULL){
        return_string = String(return_string + ",");
      }
      i++;
    }
    return_string = String(return_string + "}]");
    
    return return_string;

}

