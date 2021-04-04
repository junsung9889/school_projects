const myStorage = window.localStorage;
var user = JSON.parse(myStorage.getItem('login_user'));
if(user){
  console.log(user);
  document.getElementById("login_user").innerHTML = "Hello! "+ user.member + user.name;
}

var firebaseConfig = {
  apiKey: "AIzaSyCkJazvViVr2NMOk9dVEbHhVgnqLiji-oU",
  authDomain: "webproject-f6cf9.firebaseapp.com",
  databaseURL: "https://webproject-f6cf9-default-rtdb.firebaseio.com",
  projectId: "webproject-f6cf9",
  storageBucket: "webproject-f6cf9.appspot.com",
  messagingSenderId: "156486106438",
  appId: "1:156486106438:web:37897bb59ec1d80f953947",
  measurementId: "G-E888NHD3WH"
};
firebase.initializeApp(firebaseConfig);

var database = firebase.database();
var dbTestRef = database.ref('products')
var storage = firebase.storage();
var storageRef = storage.ref('images');
var i = 0;
var j = 0;
var k = 0;
dbTestRef.on('child_added',function(data){
  if(data){
    document.getElementsByTagName("h2")[i].innerHTML = data.val().name;
    if(data.val().status == "Auction"){
      document.getElementsByTagName("p")[j+1].innerHTML ="!!!Auction Price!!! : "+data.val().price;
    }
    else{
      document.getElementsByTagName("p")[j+1].innerHTML ="Price : "+ data.val().price;
    }
    document.getElementsByTagName("p")[j+2].innerHTML ="Trading Place : " + data.val().place;
    document.getElementsByTagName("p")[j+3].innerHTML ="Phone : " + data.val().phone;
    document.getElementsByTagName("p")[j+4].innerHTML ="Seller : " + data.val().seller;
    document.getElementsByTagName("p")[j+5].innerHTML ="Status : " + data.val().status;
    storeImage(data,k);
    i+=1;
    j+=5;
    k++;
  }
});
function storeImage(data,num){
  storageRef.child(data.val().name).getDownloadURL().then(function(url){
    if(url){
      document.getElementsByTagName("img")[num].src = url;
    }
  });
}
