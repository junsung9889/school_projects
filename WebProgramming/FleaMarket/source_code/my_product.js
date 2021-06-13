const myStorage = window.localStorage;
var user = JSON.parse(myStorage.getItem('login_user'));
if(user){
  document.getElementById("login_user").innerHTML = "Hello! "+ user.member + user.name;
  document.getElementsByTagName("h1")[0].innerHTML = user.id + "'s Products";
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
var i = 1;
var j = 0;
dbTestRef.on('child_added',function(data){
  if(data && data.val().seller == user.id){
    document.getElementsByTagName("p")[i].innerHTML = "Name : " + data.val().name;
    document.getElementsByTagName("p")[i+1].innerHTML ="Price : "+ data.val().price;
    document.getElementsByTagName("p")[i+2].innerHTML ="Place : " + data.val().place;
    document.getElementsByTagName("p")[i+3].innerHTML ="Phone : " + data.val().phone;
    document.getElementsByTagName("p")[i+4].innerHTML ="Seller : " + data.val().seller;
    document.getElementsByTagName("small")[i-1].innerHTML ="Number of People liked: " + data.val().liked.toString();
    storeImage(data,j);
    i+=5;
    j++;
  }
});
function storeImage(data,num){
  storageRef.child(data.val().name).getDownloadURL().then(function(url){
    if(url){
      document.getElementsByTagName("img")[num].src = url;
    }
  });
}


function deleteProduct(num){
  var i = 0;
  var dbTestRef = database.ref('products')
  var pkey = "";
  dbTestRef.on('child_added',function(data){
    if((i == num) && (data.val().seller == user.id)){
      pkey = data.key;
    }
    if (data.val().seller == user.id) {
      i+=1;
    }
  });
  console.log(pkey);
  var dbTestRef = database.ref('products/'+pkey).remove();
  window.alert("Delete Completed!")
}
