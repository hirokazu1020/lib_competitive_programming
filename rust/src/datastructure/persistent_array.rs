/// unimplemented
/*
実装案
参照カウンタ
trait Node {
    fn update(&self, idx: usize, val: T) -> Rc<Node>;
    fn get(&self, idx: usize) -> T;
}
impl Node for  Innner{
    [Node]
}
impl Node for  Leaf{
    [T]
}


*/

pub mod Persistent{
    const BLOCK : usize = 4;
    use std::rc::Rc;

    trait Node<T: Clone> {
        fn update(&self, idx: usize, h:usize , val: T) -> Rc<Node<T>>;
        fn get(&self, idx: usize, h:usize) -> T;
    }
    struct Inner<T: Clone>{
        child : [Rc<Node<T>>; BLOCK]
    }
    struct Leaf<T: Clone>{
        val : [T; BLOCK]
    }

    /*impl<T: Clone> Node<T> for  Inner<T>{
        //fn update(&self, idx: usize, h:usize , val: T) -> Rc<T>;
        //fn get(&self, idx: usize) -> T;
        fn update(&self, idx: usize, h:usize , val: T) -> Rc<Node<T>>{
            let mut na = self.child.clone();
            let b = (idx >> h) % BLOCK;
            na[b] = (*self.child[b]).update(idx, h - BLOCK, val);
            let node  = Inner{child :  na };
            return Rc::<Node<T>>::new(node);
        }
        fn get(&self, idx: usize, h:usize) -> T{
            let b = (idx >> h) % BLOCK;
            let ret = (*self.child[b]).get(idx, h - BLOCK);
            return ret;
        }
    }*/
    impl<T: Clone> Node<T> for  Leaf<T>{
        fn update(&self, idx: usize, h:usize , val: T) -> Rc<Node<T>>{
            let mut na = self.val.clone();
            na[idx % BLOCK] = val;
            //let leaf  = Leaf{val :  na };
            //return Rc::<Node<T>>::new(leaf);
            return Rc::<Node<T>>::new(Leaf{val :  na });
        }
        fn get(&self, idx: usize, h:usize) -> T{
            return self.val[idx & BLOCK];
        }
    }

    /*#[derive(Clone)]
    struct Array<T : Clone>{
        height : usize,
        root : Rc<Node<T>>
    }
    impl<T: Clone> Array<T>{
        pub fn new(n: usize, val : T) -> Self {
            let mut height : usize = 0;
            while (n >> height) > 0 {
                height += BLOCK;
            }
            let leaf  = Leaf{val :  [val; BLOCK] };
            let mut node = Rc::<Node<T>>::new(leaf);

            for i in 2..(height/BLOCK) {
                let x = Inner{child :  [Rc::<Node<T>>::new(node); BLOCK] };
                node = Rc::<Node<T>>::new(x);
            }

            return Array{ root  : node};
        }
        //pub fn update(&self, idx: usize, val: T) -> Rc<T>;
        //pub fn get(&self, idx: usize) -> T;
    }*/
}











/*
#[derive(Debug)]
#[allow(dead_code)]
struct PersistentArray<T>{
    //Rc::new(5);
}

#[allow(dead_code)]
impl<T: Copy> PersistentArray<T>{
    fn new () -> Self{
        let x = PersistentArray{};
        return x;
    }
}*/


fn main(){
    //let _ = PersistentArray<i32>::new();


    /*let five = Rc::new(5);
    let a = Rc::new(5);

    println!("{}", *five);
    println!("{}", *a);*/
}
