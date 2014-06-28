void SplitString(const std::string &srcStr, const std::string &splitStr, std::vector<std::string> &destVec)
{
    using namespace std;
    if ( 0 == srcStr.size())
    {
        return;
    }
    size_t oldPos = 0, newPos = 0;
    string tempData;

    while (true)
    {
        newPos = srcStr.find(splitStr, oldPos);
        if (newPos != string::npos) 
        {
            tempData = srcStr.substr(oldPos, newPos - oldPos);
            destVec.push_back(tempData);
            oldPos = newPos + splitStr.size();
        }
        else if (oldPos <= srcStr.size())
        {
            tempData = srcStr.substr(oldPos);
            destVec.push_back(tempData);
            break;
        }
        else
        {
            break;
        }
    }
}

bool IsParameterUsed(const std::vector<std::string> &parmName, const std::string &start, 
        const std::string &end, std::string &message)
{
    using namespace std;
    vector<string> resStart;
    vector<string> resEnd;
    //get filename
    SplitString(start, ":", resStart);
    SplitString(end, ":", resEnd);

    //read file
    ifstream infile(resStart[0].c_str()); 
    if (NULL == infile)
    {
        cout << "can't open file: " << resStart[0] << endl;
        return false;
    }

    string line;
    int startLine = atoi(resStart[1].c_str());
    int endLine = atoi(resEnd[1].c_str());
    int lineNo = 0;
    vector<string> body;
    string strset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    //read line by line and get token
    while (getline(infile, line))
    {
        lineNo++;
        if (lineNo > startLine && lineNo < endLine)
        {
            istringstream iss(line);
            string word;
            while(iss >> word)
            {
                int first = 0;
                while((first = word.find_first_not_of(strset)) != string::npos)
                {
                    word.erase(first, 1);
                }
                body.push_back(word);
            }
        }        
    }
    
    //test if parm is used in the file body
    typedef vector<string>::const_iterator VSCI; 
    for (VSCI i = parmName.begin(); i != parmName.end() ; i++) 
    {
        if(body.end() == find(body.begin(), body.end(), *i))
        {
            message = *i + " is unused";
            return false;
        }
    }
    return true;
} 
