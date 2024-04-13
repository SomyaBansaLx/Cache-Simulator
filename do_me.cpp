#include <iostream> 
#include <vector>

using namespace std;

const int N = 2048*2048;

int main(int argc,char* argv[]){

    vector<vector<int > > use2(N,vector <int> (4,0));

    int num_sets = stoi(argv[1]);
    int blocks_in_set = stoi(argv[2]);
    int block_size = stoi(argv[3]);
    bool write_type_is_allocate = (argv[4][0]=='w') ? 1:0 ;
    bool write_type_is_back  = (argv[5][6]=='b') ? 1:0 ;
    bool lru = (argv[6][0]=='l') ? 1:0 ;

    cout<<num_sets<<" "<<blocks_in_set<<" "<<block_size<<" "<<write_type_is_allocate<<" "<<write_type_is_back<<" "<<lru<<endl;
    
    int total_loads = 0 ;
    int total_stores = 0 ;
    int load_hits = 0 ;
    int load_miss = 0 ;
    int store_hits = 0 ;
    int store_miss = 0 ;
    int cycles  = 0 ;

    char op;
    int address;

    string line;
    int mm = 255 ;
    
    while(getline(cin,line)){

        op = line[0];
        address = 0 ;

        for(int i=4;i<12;i++){

            int use = 0 ;

            if(line[i]<='9'){

                use = line[i]-'0';

            }
            else {

                use = line[i] - 'a'+10;

            }

            address = (address<<4) + use;

        }

    int tag = (address >> ((int)log2(block_size * num_sets))) ;
    int set_index = (address >> ((int)log2(block_size))) & (num_sets-1);

    int my_time = 0 ;

    if(op=='l'){

        my_time ++;
        total_loads++;
        bool hit = false;
        int check_time = -1;
        int block_no = 0 ;
        int empty_block = -1;
        cycles++;

        for(int i=blocks_in_set*set_index;i<blocks_in_set*(set_index+1);i++){

            if(use2[i][0]==0){

                empty_block = i ;
                // break;

            }

            if(check_time==-1){

                check_time = use2[i][3];
                block_no=i;

            }

            else {

                if(use2[i][3]<check_time){
                    check_time = use2[i][3];
                    block_no = i;
                }

            }

            if(use2[i][0]==1 && use2[i][1]==tag){

                hit = true;
                load_hits++;

                if(lru){

                    use2[i][3] = my_time;

                }

                break;

            }

        }


        if (!hit) {

            load_miss++;
            cycles += 100*(block_size>>2); 

            if(empty_block>=0){

                int i = empty_block;
                use2[i][0] = 1;
                use2[i][1] = tag;
                use2[i][3] = my_time;

            }

            else {

                int i = block_no;
                use2[i][0] = 1;
                use2[i][1] = tag;
                use2[i][3] = my_time;

                if(use2[i][2]==1){

                use2[i][2] = 1 - use2[i][2];

                cycles+=100*block_size>>2;

                }

            }

            }

        }

    else {

        cycles++;
        my_time++;
        total_stores++;
        bool hit = false;
        bool dirty = false;
        int empty_block = -1;
        int check_time = -1;
        int block_no = 0 ;

        if(write_type_is_back){

            for(int i=blocks_in_set*set_index;i<blocks_in_set*(set_index+1);i++){

                if(use2[i][0]==0){

                    empty_block = i ;
                    break;

                }

                if(check_time==-1){

                    check_time = use2[i][3];
                    block_no=i;

                }

                else {

                    if(use2[i][3]<check_time){

                        check_time = use2[i][3];
                        block_no = i;

                    }

                }

                if(use2[i][0]==1 && use2[i][1]==tag){

                    hit = true;
                    store_hits++;
                    use2[i][2] = 1;

                    if(lru){

                        use2[i][3] = my_time;

                    }

                    break;

                }

            }

            if(!hit){

                store_miss++;

                if(empty_block>=0){

                    int i = empty_block;
                    use2[i][0] = 1;
                    use2[i][1] = tag;
                    // use2[i][2] = 1;
                    use2[i][3] = my_time;
                    cycles += 100*(block_size>>2); 

                }

                else {

                    int i = block_no;
                    
                    if(use2[i][2]==1){

                        cycles += 100*(block_size>>2); 

                    }

                    use2[i][0] = 1;
                    use2[i][1] = tag;
                    use2[i][2] = 0;
                    use2[i][3] = my_time;
                    cycles += 100*(block_size>>2); 

                }

        }
            
        }

        else{

            if(write_type_is_allocate){
                
                cycles+=100;

            }

            for(int i=blocks_in_set*set_index;i<blocks_in_set*(set_index+1);i++){

                if(use2[i][0]==0){

                    empty_block = i ;
                    break;

                }

                if(check_time==-1){

                    check_time = use2[i][3];
                    block_no=i;

                }
                else {

                    if(use2[i][3]<check_time){
                        check_time = use2[i][3];
                        block_no = i;
                    }

                }

                if(use2[i][0]==1 && use2[i][1]==tag){

                    hit = true;
                    store_hits++;

                    if(lru){

                        use2[i][3] = my_time;

                    }

                    break;

                }

            }


            if(!hit){

                cycles+=100*block_size>>2;

                store_miss++;

                if(write_type_is_allocate){

                    if(empty_block>=0){

                        int i = empty_block;
                        use2[i][0] = 1;
                        use2[i][1] = tag;
                        use2[i][3] = my_time;

                    }

                    else {

                        int i = block_no;

                        use2[i][0] = 1;
                        use2[i][1] = tag;
                        use2[i][3] = my_time;

                    }

            }

        }

    }

    }

    }

    std::cout<<"load hits is "<<load_hits<<endl;

    std::cout<<"load miss is "<<load_miss<<endl;

    std::cout<<"loads is "<<total_loads<<endl;

    std::cout<<"cycles is "<<cycles<<endl;

    std::cout<<"store hits is "<<store_hits<<endl;

    std::cout<<"store miss is "<<store_miss<<endl;

    std::cout<<"stores is "<<total_stores<<endl;

    return 0;

}
