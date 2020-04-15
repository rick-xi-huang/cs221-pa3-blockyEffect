
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

// Node constructor, given.
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

// twoDtree destructor, given.
twoDtree::~twoDtree(){
	clear();
}

// twoDtree copy constructor, given.
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


// twoDtree assignment operator, given.
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ 

 stats s(imIn);
 width = imIn.width();
 height = imIn.height();
 pair<int,int> ul(0, 0);
 pair<int,int> lr(width - 1, height - 1);
 root = buildTree(s, ul, lr, true);

}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {

bool vertDirection = vert;
RGBAPixel average = s.getAvg(ul, lr);
Node* node = new Node(ul, lr, average);

if(ul.first == lr.first && ul.second == lr.second){
    node->left = NULL; 
	node->right = NULL;
	return node;
}else if(ul.first == lr.first){
    vertDirection = false;
}else if(ul.second == lr.second){
    vertDirection = true;
}
     	 
if(vertDirection){

    pair<int,int> uln(ul.first + 1, ul.second);
    pair<int,int> lrn(ul.first, lr.second);
	  long sumDevMin = s.getScore(ul, lrn) + s.getScore(uln, lr); 

	 for (int i = ul.first; i < lr.first; i++){
		 pair<int,int> lrt(i, lr.second);
		 pair<int,int> ult(i + 1, ul.second);
         long sumDev = s.getScore(ul, lrt) + s.getScore(ult, lr);
         if(sumDev <= sumDevMin){
             uln.first = i + 1;
			 lrn.first = i;
			 sumDevMin = sumDev;
		 }
	 }
	
	vertDirection = !vertDirection;
	node->left = buildTree(s, ul, lrn, vertDirection);
	node->right = buildTree(s, uln, lr, vertDirection);
   
}else{

    pair<int,int> uln(ul.first, ul.second + 1);
    pair<int,int> lrn(lr.first, ul.second);
	  long sumDevMin = s.getScore(ul, lrn) + s.getScore(uln, lr); 

	 for (int j = ul.second; j < lr.second; j++){
		  pair<int,int> lrt(lr.first, j);
	    pair<int,int> ult(ul.first, j + 1);
      long sumDev = s.getScore(ul, lrt) + s.getScore(ult, lr);
         if(sumDev <= sumDevMin){
             uln.second = j + 1;
			       lrn.second = j;
			       sumDevMin = sumDev;
		 }
	 }
	
	vertDirection = !vertDirection;
	node->left = buildTree(s, ul, lrn, vertDirection);
	node->right = buildTree(s, uln, lr, vertDirection);

}
  
  return node;
   
}

PNG twoDtree::render(){

  PNG img;

  img.resize(width,height);

  renderPixel(root, img);
  
  return img;

}

void twoDtree::renderPixel(const Node * subRoot, PNG & im){

  if(subRoot->left == NULL && subRoot->right == NULL){
    int ulx = subRoot->upLeft.first;
    int uly = subRoot->upLeft.second;
    int lrx = subRoot->lowRight.first;
    int lry = subRoot->lowRight.second;
    for (int i = ulx; i <= lrx; i++){
       for (int j = uly; j <= lry; j++){
            *im.getPixel(i, j) = subRoot->avg;
       }
    }
  }
  
  if(subRoot->left != NULL){
    renderPixel(subRoot->left, im);
  }

  if(subRoot->right != NULL){
    renderPixel(subRoot->right, im);
  }

}

int twoDtree::idealPrune(int leaves){

   int tol = 0;
   int result = pruneSize(tol);
   if (result <= leaves){
     return tol;
   }
   while(result > leaves){
      tol = tol + 1000000;
      result = pruneSize(tol);
   }
   tol = tol - 1000000;
   result = pruneSize(tol);
   while(result > leaves){
      tol = tol + 100000;
      result = pruneSize(tol);
   }
   tol = tol - 100000;
   result = pruneSize(tol);
   while(result > leaves){
      tol = tol + 10000;
      result = pruneSize(tol);
   }
   tol = tol - 10000;
   result = pruneSize(tol);
   while(result > leaves){
      tol = tol + 1000;
      result = pruneSize(tol);
   }
   tol = tol - 1000;
   result = pruneSize(tol);
   while(result > leaves){
      tol = tol + 100;
      result = pruneSize(tol);
   }
   tol = tol - 100;
   result = pruneSize(tol);
   while(result > leaves){
      tol = tol + 10;
      result = pruneSize(tol);
   }
   tol = tol - 10;
   result = pruneSize(tol);
   while(result > leaves){
      tol++;
      result = pruneSize(tol);
   }
   return tol;
}

int twoDtree::pruneSize(int tol){
    
  int ret = countLeaves(root, tol);
  return ret;

}

void twoDtree::prune(int tol){

  clearNode(root, tol);

}

void twoDtree::clearNode(Node *& subRoot, int tol){

 if(subRoot != NULL){
  if(checkLeaves(subRoot->avg, subRoot, tol)){
     deleteNode(subRoot->left);
     deleteNode(subRoot->right);
     subRoot->left = NULL;
     subRoot->right = NULL;
  }else{
	 clearNode(subRoot->left, tol);
	 clearNode(subRoot->right, tol);
  }
 }
}

bool twoDtree::checkLeaves(RGBAPixel average, const Node * subRoot, int tol){

 if(subRoot->left == NULL && subRoot->right == NULL){

   int diff = (subRoot->avg.r - average.r) * (subRoot->avg.r - average.r) 
              + (subRoot->avg.g - average.g) * (subRoot->avg.g - average.g)
			  + (subRoot->avg.b - average.b) * (subRoot->avg.b - average.b);
   return (diff <= tol);
   
 }else{

  return checkLeaves(average, subRoot->left, tol) && checkLeaves(average, subRoot->right, tol);

 }
}

int twoDtree::countLeaves(const Node * subRoot, int tol){

int ret = 0;

 if(subRoot != NULL){
  if(checkLeaves(subRoot->avg, subRoot, tol)){
     ret = 1;
  }else{
	 ret = countLeaves(subRoot->left, tol) + countLeaves(subRoot->right, tol);
  }
 }
return ret;
}

void twoDtree::clear() {

deleteNode(root);

}

void twoDtree::deleteNode(Node *& subRoot){

if(subRoot == NULL){
  return;
}

deleteNode(subRoot->left);
deleteNode(subRoot->right);

delete subRoot;
subRoot = NULL;

}

void twoDtree::copy(const twoDtree & orig){

width = orig.width;
height = orig.height;
root = copyTree(orig.root);

}


twoDtree::Node * twoDtree::copyTree(const Node * other){

    if(other != NULL){
    Node * node = new Node(other->upLeft, other->lowRight, other->avg);
    node->left = copyTree(other->left);
    node->right = copyTree(other->right);
	  return node;
	}else{
    return NULL;
	}
}



