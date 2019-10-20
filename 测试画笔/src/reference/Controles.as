package
{
	import flash.events.MouseEvent;
	import flash.display.MovieClip;
	
	/**
	 * @author kris@neuroproductions.be
	 */
	public class Controles extends Controle_assets
	{
		public static var BRUSHTYPE:int = 1;
		
		public function Controles()
		{
			super();
			UndoBtn.mouseChildren = false;
			UndoBtn.buttonMode = true;
			
			clearBtn.mouseChildren = false;
			clearBtn.buttonMode = true;
			btn1.y = -10;
			btn1.mouseChildren = false;
			btn1.buttonMode = true;
			
			btn2.mouseChildren = false;
			btn2.buttonMode = true;
			
			btn3.mouseChildren = false;
			btn3.buttonMode = true;
			
			btn4.mouseChildren = false;
			btn4.buttonMode = true;
			
			btn5.mouseChildren = false;
			btn5.buttonMode = true;
			btn5.visible = false;
			btn1.addEventListener(MouseEvent.CLICK, setBtn);
			btn2.addEventListener(MouseEvent.CLICK, setBtn);
			btn3.addEventListener(MouseEvent.CLICK, setBtn);
			btn4.addEventListener(MouseEvent.CLICK, setBtn);
			btn5.addEventListener(MouseEvent.CLICK, setBtn);
		}
		
		private function setBtn(event:MouseEvent):void
		{
			btn1.y = btn2.y = btn3.y = btn4.y = btn5.y = 0;
			var mc:MovieClip = event.currentTarget as MovieClip;
			mc.y = -10;
			if (mc == btn1)
				BRUSHTYPE = 1;
			if (mc == btn2)
				BRUSHTYPE = 2;
			if (mc == btn3)
				BRUSHTYPE = 3;
			if (mc == btn4)
				BRUSHTYPE = 4;
			if (mc == btn5)
				BRUSHTYPE = 5;
		
		}
	}
}
