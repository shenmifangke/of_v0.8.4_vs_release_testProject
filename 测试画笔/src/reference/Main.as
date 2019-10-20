package
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.BlendMode;
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.display.TriangleCulling;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	/**
	 * 2013/4/7 18:52
	 * @author W.J
	 * 透明度绘画问题 已修复 为重复画自己的问题
	 * 内存主要占用提高为 撤销的开销
	 */
	public class Main extends Sprite
	{
		
		[Embed(source="Fbrushshadow2.png")]
		public var Frush1:Class;
		
		[Embed(source="Sbrush2.png")]
		public var Frush2:Class;
		
		[Embed(source="flowerBrush.png")]
		public var Frush3:Class;
		
		[Embed(source="brush4.png")]
		public var Frush4:Class;
		
		[Embed(source="brush5.png")]
		public var Frush5:Class;
		
		[Embed(source="Fbrushmask2.png")]
		public var FrushMask:Class;//遮罩
		
		[Embed(source="papier2.jpg")]
		public var Papier:Class;
		
/*		//平铺图案
		[Embed(source="117.jpg")]
		public var Patern:Class;*/
		
		private var linePoints:Array = new Array();//点的数组
		private var linePointsInput:Array = new Array();
		private var controles:Controles = new Controles();//按钮控制器
		
		private var holder:Sprite = new Sprite();//画笔临时
		private var holderS:Sprite = new Sprite();//笔触1的shadow纹理
		private var holderM:Sprite = new Sprite();//笔触1遮罩mask缓存纹理
		private var holderP:Sprite = new Sprite();//缓存纹理page
		
		private var back:Sprite = new Sprite();//背景元件 响应画笔按压事件
		private var bmdScreen:BitmapData;//背景画板
		private var bmScreen:Bitmap;
		
		private var bmP:Bitmap;//画笔1的纸张背景
		private var undo_arr:Array = new Array();//缓存位图用的数组
		public function Main():void
		{
			if (stage)
				init();
			else
				addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		
		
		private function init(e:Event = null):void
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			var bm1:Bitmap = new Frush1() as Bitmap;
			bmdB1 = bm1.bitmapData;
			
			var bm2:Bitmap = new Frush2() as Bitmap;
			bmdB2 = bm2.bitmapData;
			
			var bm3:Bitmap = new Frush3() as Bitmap;
			bmdB3 = bm3.bitmapData;
			
			var bm4:Bitmap = new Frush4() as Bitmap;
			bmdB4 = bm4.bitmapData;
			
			var bm5:Bitmap = new Frush5() as Bitmap;
			bmdB5 = bm5.bitmapData;
			
			var bmM:Bitmap = new FrushMask() as Bitmap;
			bmdMask = bmM.bitmapData;
			
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			this.addChild(back);
			
			bmP = new Papier() as Bitmap;
			bmP.smoothing = true;
			
			holderP.addChild(bmP);//加入画笔1的纸张背景 holder Page
			
			holderP.cacheAsBitmap = true;//画笔纹理 包括笔刷1实际上是阴影的里面加入了位图填充
			holderM.cacheAsBitmap = true;//背景填充纹理
			
			holder.addChild(holderP);
			holder.addChild(holderM);
			holderP.mask = holderM;//给纸张纹理加上遮罩 实际上的遮罩是g2上的图形 也就是画出来的
			
			holder.addChild(holderS);//纸张纹理的外围 也和其他三支笔有关
			
			//holderS.blendMode = BlendMode.MULTIPLY;
			
			bmdScreen = new BitmapData(2000, 1000, true, 0);
			bmScreen = new Bitmap(bmdScreen);
			//this.addChild(bmScreen);
			this.addChild(holder);//后加入画笔
			
			back.addChild(bmScreen);
			back.addEventListener(MouseEvent.MOUSE_DOWN, startDraw);//背景元件 响应画笔按压事件
			
			this.addChild(controles);//GUI
			
			controles.UndoBtn.addEventListener(MouseEvent.CLICK, doUndo);
			controles.clearBtn.addEventListener(MouseEvent.CLICK, doClear);
			stage.addEventListener(Event.RESIZE, doResize);
			doResize();
		}
		
		//撤销
		private function doUndo(event:MouseEvent):void
		{
			if (undo_arr.length > 0)
			{
				var bmS:BitmapData = undo_arr.pop();
				bmScreen.bitmapData = bmS;
			}
		}
		//清空画板
		private function doClear(event:MouseEvent):void
		{
			//bmScreen.bitmapData.draw(this);
			undo_arr.push(bmScreen.bitmapData.clone());//保存清空的那一步
			bmScreen.bitmapData = new BitmapData(stage.stageWidth, stage.stageHeight, true, 0);
		}
		//变化舞台时候适配屏幕
		private function doResize(event:Event = null):void
		{
			try
			{
				controles.x = 20;
				controles.y = stage.stageHeight - 30;
				
				//var bmpat:Bitmap = new Patern() as Bitmap;//获取平铺图案
				//var bmdpat:BitmapData = bmpat.bitmapData;
				
				//背景相应区域初始化
				//back.graphics.clear();
				//back.graphics.beginBitmapFill(bmdpat, null, true, false);
				//back.graphics.drawRect(0, 0, 2500, 1500);
				
				//纸张背景
				bmP.width = stage.stageWidth;
				bmP.height = stage.stageHeight;
				
				//画板
				bmdScreen = new BitmapData(stage.stageWidth, stage.stageHeight, true, 0);
				bmdScreen.draw(bmScreen)
				bmScreen.bitmapData = bmdScreen;
				//包含画板的容器
				bmScreen.cacheAsBitmap = true;//保存上一次下笔的图案
			}
			catch (e:Error)
			{
			}
		}
		//鼠标位置
		var xPos:Number;
		var yPos:Number;
		private function startDraw(event:MouseEvent):void
		{
			//对画笔1中笔触阴影模式的处理
			if (Controles.BRUSHTYPE == 1)
			{
				holderS.blendMode = BlendMode.MULTIPLY;
			}
			else
			{
				holderS.blendMode = BlendMode.NORMAL;
			}
			
			xPos = mouseX;
			yPos = mouseY;
			var lp:LinePoint = new LinePoint(xPos, yPos);
			linePoints.push(lp);
			
			this.addEventListener(MouseEvent.MOUSE_MOVE, draw);
			stage.addEventListener(MouseEvent.MOUSE_UP, stopDraw);
		}
		
		
		
		private function stopDraw(event:MouseEvent):void
		{
			this.removeEventListener(MouseEvent.MOUSE_MOVE, draw);
			stage.removeEventListener(MouseEvent.MOUSE_UP, stopDraw);
			controles.visible = false;
			undo_arr.push(bmScreen.bitmapData.clone());
			if (undo_arr.length > 10)
			{
				var bmdS:BitmapData = undo_arr.shift();
				bmdS.dispose();
			}
			//bmScreen.bitmapData.draw(this);
			bmScreen.bitmapData.draw(holder);
			controles.visible = true;
			/*holderS.visible =true;
			 bmScreen.bitmapData.draw(holderS,null,null,BlendMode.MULTIPLY);*/
			clear();
		}
		

		
		private function draw(event:Event):void
		{
			if (Controles.BRUSHTYPE == 2)
			{
				//画笔延迟
				xPos += (mouseX - xPos) / 5;
				yPos += (mouseY - yPos) / 5;
				var lp:LinePoint = new LinePoint(xPos, yPos, linePoints[linePoints.length - 1]);
			}
			else
			{
				//其他没有延迟
				xPos = mouseX;
				yPos = mouseY;
				var lp:LinePoint = new LinePoint(xPos, yPos, linePoints[linePoints.length - 1]);
			}
			
			linePointsInput.push(lp);//压入新的坐标
			
			calculate();//计算
			render();//进行填充
			
		}
		
		private var lastLength:Number = 0;//上一次的长度
		private var segLength:Number = 0;//达到填充的分段数
		
		private function calculate():void
		{
			//分段间隔
			if (Controles.BRUSHTYPE == 2)
				segLength = 50;
			if (Controles.BRUSHTYPE == 3)
				segLength = 20;
			lastLength = 0;
			linePoints = new Array();
			linePoints.push(linePointsInput[0]);
			
			vertices = new Vector.<Number>();
			indices = new Vector.<int>();
			uvtData = new Vector.<Number>();
			for (var i:int = 1; i < linePointsInput.length; i++)
			{
				var lp:LinePoint = linePointsInput[i];
				
				//trace("lp.currentLength="+lp.currentLength+" lastLength="+lastLength+" segLength="+segLength)
				//当前笔触移动了足够的距离才开始画
				if (lp.currentLength > lastLength + segLength)//节点当前长度比上次长度和分段数大的时候
				{
					//trace(lp.currentLength);
					lastLength = lp.currentLength;
					linePoints.push(lp);
				}
				
			}
			
			var count = 0;
			var uvStep:Number = 1 / linePoints.length;
			
			//画笔1，4不固定
			if (Controles.BRUSHTYPE == 2)
			{
				uvStep = 0.010;//固定分段
			}
			if (Controles.BRUSHTYPE == 3)
			{
				uvStep = 0.15;
			}
			var currentUV:Number = 0;
			
			for (var i:int = 0; i < linePoints.length - 1; i += 2)//加2就完整了 加1不完整
			{
				
				var lp:LinePoint = linePoints[i];
				//三角形连续坐标
				vertices.push(lp.xL, lp.yL, lp.xR, lp.yR);//数组向量压入笔触宽度两端坐标 前一个
				lp = linePoints[i + 1];
				vertices.push(lp.xL, lp.yL, lp.xR, lp.yR);//数组向量压入笔触宽度两端坐标 当前
				
				//点索引顺序
				indices.push(count, count + 1, count + 2, count + 1, count + 2, count + 3);
				indices.push(count + 2, count + 3, count + 4, count + 3, count + 4, count + 5);
				//和vertices必须对应
				uvtData.push(currentUV, 0, currentUV, 1, currentUV + uvStep, 0, currentUV + uvStep, 1);
				count += 4;
				currentUV += uvStep;
				currentUV += uvStep;
			}
		}
		
		private var vertices:Vector.<Number>;
		private var indices:Vector.<int>;
		private var uvtData:Vector.<Number>;
		private var bmdB1:BitmapData;
		private var bmdB2:BitmapData;
		private var bmdB3:BitmapData;
		private var bmdB4:BitmapData;
		private var bmdB5:BitmapData;
		private var bmdMask:BitmapData;//其实就是绘制的一个形状
		
		private function render():void
		{
			holderS.graphics.clear();
			//填充位图
			if (Controles.BRUSHTYPE == 1)
			{
				holderS.graphics.beginBitmapFill(bmdB1, null, false, true)
			}
			if (Controles.BRUSHTYPE == 2)
			{
				holderS.graphics.beginBitmapFill(bmdB2, null, true, true)
				
			}
			if (Controles.BRUSHTYPE == 3)
			{
				holderS.graphics.beginBitmapFill(bmdB3, null, true, true)
			}
			if (Controles.BRUSHTYPE == 4)
			{
				holderS.graphics.beginBitmapFill(bmdB4, null, true, true)
			}
			if (Controles.BRUSHTYPE == 5)
			{
				holderS.graphics.beginBitmapFill(bmdB5, null, true, true)
			}
			//holderS.graphics.lineStyle(1, 0);
			//三角形填充
			holderS.graphics.drawTriangles(vertices, indices, uvtData, TriangleCulling.NONE);
			
			//内部纸张
			if (Controles.BRUSHTYPE == 1)
			{
				holderM.graphics.clear();
				
				holderM.graphics.beginBitmapFill(bmdMask, null, false, true);//笔触形状
				holderM.graphics.drawTriangles(vertices, indices, uvtData, TriangleCulling.NONE);//和纸张画笔有关
			}
		}
		
		private function clear():void
		{

			holderS.graphics.clear();
			holderM.graphics.clear();
			LinePoint.fullLength = 0;
			linePointsInput = new Array();
			linePoints = new Array();
			
			vertices.length = 0;
			indices.length = 0;
			uvtData.length = 0;
		}
	}

}