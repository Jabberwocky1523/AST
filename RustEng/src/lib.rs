mod audio;
mod config;
mod error;
mod executor;
mod parser;
mod run;
mod script;
mod ui;

use crate::error::EngineError;
use crate::run::{build, start};
#[no_mangle]
pub extern "C" fn test(){
    start();
    println!("test");
}
#[no_mangle]
pub extern "C" fn test2(){
    println!("rust");
}