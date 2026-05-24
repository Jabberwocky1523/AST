use crate::audio::player::Player;
use crate::error::EngineError;
use crate::script::Script;
use crate::ui::ui::ui;
use std::cell::RefCell;
use std::rc::Rc;

pub async fn build() -> Result<(), EngineError> {
    let mut script = Script::new();
    script.with_name("ky01")?;
    let script = Rc::new(RefCell::new(script));
    let bgm_player = Rc::new(RefCell::new(Player::new()));
    let voice_player = Rc::new(RefCell::new(Player::new()));
    //println!("{:#?}", script);
    ui(script, bgm_player, voice_player).await?;
    Ok(())
}

pub fn start() {
    let rt = tokio::runtime::Runtime::new().expect("Failed to create Tokio runtime");
    rt.block_on(async {
        let mut script = Script::new();
        script.with_name("ky01").expect("Failed to set script name");
        let script = Rc::new(RefCell::new(script));
        let bgm_player = Rc::new(RefCell::new(Player::new()));
        let voice_player = Rc::new(RefCell::new(Player::new()));
        //println!("{:#?}", script);
        if let Err(e) = ui(script, bgm_player, voice_player).await {
            eprintln!("UI error: {:?}", e);
        }
    });
}