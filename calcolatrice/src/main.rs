use std::io;
fn main() {
    loop {
        let mut cifra: String = String::new();
        let mut cifra2: String = String::new();
        io::stdin().read_line(&mut cifra).expect("error");
        let cifra: i32 = match cifra.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        io::stdin().read_line(&mut cifra2).expect("error");
        let cifra2: i32 = match cifra2.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        let ris: i32 = cifra + cifra2;
        let ris: String = ris.to_string();
        println!("{ris}");
        println!("vuoi continuare? 1:si 2:no");
        let mut scelta = String::new();
        io::stdin().read_line(&mut scelta).expect("error");
        let scelta: i32 = match scelta.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        if scelta == 1 {
        } else {
            break;
        }
    }
}
