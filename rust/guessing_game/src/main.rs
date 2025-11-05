use std::cmp::Ordering;
use std::io;
use rand::Rng;
fn main() {
    println!("indovina il numero");
    let secret_number = rand::thread_rng().gen_range(1..=100);
    loop{
    println!("inserisci l'input");
    let mut  guess=String::new();
    io::stdin()
        .read_line(&mut guess)
        .expect("input sbagliato");
    let guess: u32 = match guess.trim().parse() {
        Ok(num)=>num,
        Err(_)=> continue,
    };

    println!("hai inserito: {guess}");
    match guess.cmp(&secret_number){
        Ordering::Less => println!("Too small"),
        Ordering::Greater => println!("too big"),
        Ordering::Equal =>{ 
            println!("you win");
            break;
            }
        }
    }
}
