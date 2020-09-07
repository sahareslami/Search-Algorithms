#include <iostream>


using namespace std;
const int N = 2 * 1e6 + 7;last

int main(){
	int t;
	cin >> t;
	while(t--){
		int a[N];
		int n;
		cin >> n;
		for(int i = 0 ; i < n ; i++)
			cin >> a[i];
		int ans = 0;
		int last = -20;
		for(int i = 0 ; i < n ; i++){
			if(abs(last - a[i]) >= 10){
				ans++;
				last = a[i] + 10;
			}
		}
		cout << ans << endl;
	}

}