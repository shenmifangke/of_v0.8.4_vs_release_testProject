package
{
	import flash.geom.Point;
	
	/**
	 * @author kris@neuroproductions.be
	 */
	public class LinePoint extends Point
	{
		public static var fullLength:Number = 0;
		public var currentLength:Number;//节点当前距离
		public var l:Number;//与上一个节点之间的距离
		public var xL:Number;
		public var yL:Number;
		public var xR:Number;
		public var yR:Number;
		public var angle:Number = 0;
		public var angleT:Number = 0;
		public var prevPoint:LinePoint
		public var lw:Number = 30;//线条宽度lineWidth
		
		public function LinePoint(xPos:Number, yPos:Number, prevPoint:LinePoint = null)
		{
			super(xPos, yPos);
			this.prevPoint = prevPoint;
			if (prevPoint)
			{
				l = Point.distance(this, prevPoint);//与上一个节点之间的长度
				
				fullLength += l;//节点总长度
				currentLength = fullLength;//节点当前长度
				
				if (Controles.BRUSHTYPE == 2)//第二种画笔的宽度是随着速度变化的
					lw = (l + prevPoint.l) / 2;
				if (Controles.BRUSHTYPE == 3)
					lw = 30;
				angleT = Math.atan2(this.y - prevPoint.y, this.x - prevPoint.x);//角度弧度值
				angle = angleT;
				
				//做直线左右两边的等长垂线
				var pR:Point = Point.polar(lw, angle + Math.PI / 2);//将一对极坐标转换为笛卡尔点坐标
				xR = pR.x + x;
				yR = pR.y + y;
				var pL:Point = Point.polar(lw, angle - Math.PI / 2);
				xL = pL.x + x;
				yL = pL.y + y;
				
				//可以去掉 似乎是宽度控制
				if (prevPoint.prevPoint)
				{
					//prevPoint.setAngel(this);
				}
			}
			else
			{
				xR = x;
				yR = y;
				
				xL = x;
				yL = y;
			}
		}

	}
}
