#include <iostream>
#include <cstring>
#include <fstream>

#define INSERT_COST 1
#define DELETE_COST 1
#define REPLACE_COST 1

#define QUERY_FILE "queries.txt"
#define TWEET_FILE "train.txt"

using namespace std;

/*
class Index{
   private:
      int qIndex, tIndex;
   public:
      Index();
      int setIndex(int a, int b){
         qIndex = a;
         tIndex = b;
      }

      int getQIndex(){
         return qIndex;
      }

      int getTIndex(){
         return tIndex;
      }
};
*/


int max(int a,int b) {
   return ((a > b) ? a : b);
}

/* convert str1 to str2 with minimum edits(insert,delete,replace)
   suppose length(str1) = m and length(str2) = n
   cost(i,j) -> cost of converting str1[0...i-1] to str2[0...j-1]
   cost(m,n) -> cost of converting str1 to str2 
   Standard recursive formula for computing cost(m,n) :-
   cost(i,j) = min[cost(i-1,j)+D, cost(i,j-1)+I, cost(i-1,j-1)+R]
   D -> delete cost, I -> insert cost, R -> replace cost  */

int editDistance(char str1[],int size1,char str2[],int size2) {
   // cost matrix
   // row -> str1 & col -> str2
   int cost[size1][size2];
   int i,j;
   int isEqual = 0;
   int countD = 1;
   int previous_dist = 0;
   int qIndex = 0, tIndex = 0;
   //Index finder;

   // initialize the cost matrix
   for (i=0;i<size1;i++) {
      for(j=0;j<size2;j++) {
         if (i == 0) {
            // source string is NULL
            // so we need 'j' insert operations
            cost[i][j] = 0;
         } else if (j == 0) {
            // target string is NULL
            // so we need 'i' delete operations
            cost[i][j] = 0;
         } else {
            cost[i][j] = -1;
         }
      }
   } //initialization done

   //compute cost(i,j) and eventually return cost(m,n)
   for(i=1;i<size1;i++) {
      for(j=1;j<size2;j++) {
         int x = cost[i-1][j] - DELETE_COST;
         int y = cost[i][j-1] - INSERT_COST;
         // if str1(i-1) != str2(j-1), add the replace cost
         // we are comparing str1[i-1] and str2[j-1] since
         // the array index starts from 0
         if(str1[i-1] != str2[j-1])
            isEqual = -1;
         else
            isEqual = 1;

         int z = cost[i-1][j-1] + (isEqual)*REPLACE_COST;
         // as per our recursive formula
         cost[i][j] = max(max(0, x), max(y,z));

         if(countD == 1){
            previous_dist = cost[i][j];
         }
         if(countD > 1){
            if(cost[i][j] > previous_dist){
               previous_dist = cost[i][j];
               //finder.setIndex(i,j);
            }
         }
         countD++;

         //cout << "cost[" << i << "][" << j << "] is: " << previous_dist << endl;
      }
      countD = 1;
   }

   //cout << "size1 is: " << size1 << "size2 is: " << size2 << endl;
   // last cell of the matrix holds the answer
   //return cost[size1-1][size2-1];
   //cout << "Index = [" << qIndex << "][" << tIndex << "]";
   return previous_dist;
}

//main
int main() {
   std::clock_t start;
   double duration;
   start = std::clock();

   std::string tweet, query;
   //char tweetplus = '\0', queryplus = '\0';

   std::ifstream tweetFile;
   std::ifstream queryFile;

   queryFile.open(QUERY_FILE);
   tweetFile.open(TWEET_FILE);

/*
      std::getline(queryFile, query);
      char * str1 = new char [query.length()+1];
      std::strcpy (str1, query.c_str());
      std::getline(tweetFile, tweet);
      char * str2 = new char [tweet.length()+1];
      std::strcpy (str2, tweet.c_str());
      int size1 = strlen(str1);
      int size2 = strlen(str2);
      int previous_min_cost = editDistance(str1,size1+1,str2,size2+1);
      int new_min_cost;*/

   int count = 1;
   int size1 = 0, size2 = 0;
   int previous_cost = 0;
   int max_cost = 0;
   //Index finder;

   while (std::getline(queryFile, query))
   {
      char * str1 = new char [query.length()+1];
      std::strcpy (str1, query.c_str());

      while(std::getline(tweetFile, tweet)){

         char * str2 = new char [tweet.length()+1];
         std::strcpy (str2, tweet.c_str());

         size1 = strlen(str1);
         size2 = strlen(str2);
         max_cost = editDistance(str1,size1+1,str2,size2+1);

         //cout<< max_cost << ", from [" << str1 << "]" << "to [" << str2 << "]" << endl;
 
 
         if(count == 1){
            previous_cost = max_cost;
         }
         if(count > 1){
            if(max_cost > previous_cost){
               if(max_cost < 10)
                  cout<< "0" << max_cost << ", from [" << str1 << "] to [" << str2 << "]";
               else
                  cout<< max_cost << ", from [" << str1 << "] to [" << str2 << "].";
               //cout << "Index = [" << finder.getQIndex() << "][" << finder.getTIndex() << "]";
               cout<<endl;
               previous_cost = max_cost;
            }
         }
         count++;
     
      }
      count = 1;
      tweetFile.close();
      tweetFile.open(TWEET_FILE);
   }
   
   tweetFile.close();
   queryFile.close();

   duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
   std::cout<<"Duration: "<< duration <<'\n';

   return 0;

}
