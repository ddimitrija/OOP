#include <bits/stdc++.h>
using namespace std;

int main(){
    ofstream out("photo.ppm");

    out << "P3" << '\n' ;
    out << "1024 1024" << '\n';
    out << "255" << '\n';

    int x0=512 , y0=512;
    int maxR=512;
    for(int i=0;i<1024;i++){
        for(int j=0;j<1024;j++){
            int x = i;
            int y = j;
            double distance=sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0));

            if(distance>maxR){
                out<< "0 0 0" << '\n';
                continue;
            }

            double normr=1-(distance/maxR);
            int r=round(normr*255);
            if(r>255) r=255;
            if(r<0) r=0;
            out << r << " 0 0"<< '\n';
        }
    }
    return 0;
}
