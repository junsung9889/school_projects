const myStorage = window.localStorage;
var user = JSON.parse(myStorage.getItem('login_user'));
if(user){
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
let s = myStorage.getItem('S');
var size = 0;
dbTestRef.on('child_added',function(data){size++;});

var auctionProducts = [];

var i = 25;
var j = 7;
var k = 4;
var l = 1;
dbTestRef.on('child_added',function(data){
  if(data){
    if(data.val().name == s || data.val().price == s || data.val().place == s || data.val().phone == s || data.val().seller == s){
      document.getElementsByTagName("p")[1].innerHTML = "Name : " + data.val().name;
      if(data.val().status == "Auction"){
        document.getElementsByTagName("p")[2].innerHTML ="!!!Auction Price!!! : "+data.val().price+"X $"+ (parseInt(data.val().price.substring(1,))/2).toString();
      }
      else{
        document.getElementsByTagName("p")[2].innerHTML ="Price : "+ data.val().price;
      }
      document.getElementsByTagName("p")[3].innerHTML ="Place : " + data.val().place;
      document.getElementsByTagName("p")[4].innerHTML ="Phone : " + data.val().phone;
      document.getElementsByTagName("p")[5].innerHTML ="Seller : " + data.val().seller;
      document.getElementsByTagName("p")[6].innerHTML ="Status : " + data.val().status;
      storageRef.child(data.val().name).getDownloadURL().then(function(url){
        if(url){
          document.getElementsByTagName("img")[0].src = url;
        }
      });
    }
    if(data.val().status == "Auction"){
      document.getElementsByTagName("p")[j].innerHTML = "Name : " + data.val().name;
      document.getElementsByTagName("p")[j+1].innerHTML ="!!!Auction Price!!! : "+data.val().price;
      document.getElementsByTagName("p")[j+2].innerHTML ="Place : " + data.val().place;
      document.getElementsByTagName("p")[j+3].innerHTML ="Phone : " + data.val().phone;
      document.getElementsByTagName("p")[j+4].innerHTML ="Seller : " + data.val().seller;
      document.getElementsByTagName("p")[j+5].innerHTML ="Status : " + data.val().status;
      storeImage(data,l);
      j+=6;
      l++;
      auctionProducts.push(data);
    }
    document.getElementsByTagName("p")[i].innerHTML = "Name : " + data.val().name;
    document.getElementsByTagName("p")[i+1].innerHTML ="Price : "+ data.val().price;
    document.getElementsByTagName("p")[i+2].innerHTML ="Place : " + data.val().place;
    document.getElementsByTagName("p")[i+3].innerHTML ="Phone : " + data.val().phone;
    document.getElementsByTagName("p")[i+4].innerHTML ="Seller : " + data.val().seller;
    document.getElementsByTagName("p")[i+5].innerHTML ="Status : " + data.val().status;
    storeImage(data,k);
  }
  i+=6;
  k++
});
function storeImage(data,num){
  storageRef.child(data.val().name).getDownloadURL().then(function(url){
    if(url){
      document.getElementsByTagName("img")[num].src = url;
    }
  });
}

function search(){
  s = document.getElementById('searchinput').value;
  myStorage.setItem('S',s);
}

var dbTestRef1 = database.ref();
function addCart(num){
  var i = 0;
  dbTestRef.on('child_added',function(data){
    if(num == 999 && data.val().name == s && data.val().status != "Purchased"){
      dbTestRef1.child(user.id+"_cart").push({name:data.val().name,price:data.val().price});
      window.alert("Add to Cart Success!");
    }
    if(i == num && data.val().status != "Purchased"){
      dbTestRef1.child(user.id+"_cart").push({name:data.val().name,price:data.val().price});
      window.alert("Add to Cart Success!");
    }
    i++;
  });
}
function addWish(num){
  var i = -1;
  dbTestRef.on('child_added',function(data){
    i++;
    if(num == 999 && data.val().name == s){
      dbTestRef1.child(user.id+"_wish").push({name:data.val().name,price:data.val().price});
      wishUpdates(data);
    }
    if(i == num){
      dbTestRef1.child(user.id+"_wish").push({name:data.val().name,price:data.val().price});
      wishUpdates(data);
    }
  });
}
function wishUpdates(data){
  var updates = {};
  updates['liked'] = data.val().liked + 1;
  dbTestRef.child(data.key).update(updates);
  window.alert("Add to Wish list Success!");
}

function biding(num){
  var pprice = document.getElementById("price");
  var data = auctionProducts[num];
  var updates = {};
  var after = parseInt(pprice.value.substring(1,));
  if(after > parseInt(data.val().price.substring(1,))){
    updates['price'] = "$"+(after).toString();
    dbTestRef.child(data.key).update(updates);
    window.alert("Bidding Success!");
  }
  else{
    window.alert("Bidding Failed! Suggest higher price!");
  }
  location.reload();
}
