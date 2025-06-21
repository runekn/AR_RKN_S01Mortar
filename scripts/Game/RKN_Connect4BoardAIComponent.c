[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class RKN_Connect4BoardAIComponentClass : ScriptComponentClass
{
}

class RKN_Connect4BoardAIComponent : ScriptComponent
{
	[Attribute(defvalue: "5")]
	int m_iMinMaxDepth;
	
	[Attribute(defvalue: "2")]
	int m_iAIPlayer;
	
	[Attribute(defvalue: "1")]
	int m_iOpponentPlayer;
	
	[Attribute(defvalue: "3")]
	int m_iDelay;
	
	RKN_Connect4BoardComponent m_Board;
	bool m_bMyTurn;
	bool m_bOpponentCheating;
	const float INFINITY = 999999999; // float.INFINITY causes error when testing in workshop. So just use big number instead.
	
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		m_Board = RKN_Connect4BoardComponent.Cast(owner.FindComponent(RKN_Connect4BoardComponent));
		m_Board.m_OnPlayerMove.Insert(StartSpiritTimer);
		m_Board.m_OnReset.Insert(Reset);
	}
	
	override protected void EOnDeactivate(IEntity owner)
	{
		super.EOnDeactivate(owner);
		m_Board.m_OnPlayerMove.Remove(StartSpiritTimer);
	}
	
	void Reset()
	{
		m_bOpponentCheating = false;
		if (m_bMyTurn)
			SCR_ScenarioFrameworkSystem.GetCallQueuePausable().Remove(TryMakeMove);
		m_bMyTurn = false;
	}
	
	void StartSpiritTimer()
	{
		if (GetGame().GetPlayerManager().GetPlayerCount() > 1)
			return;
		
		if (m_Board.m_iWinner != 0 || m_bOpponentCheating)
			return;
		
		if (m_bMyTurn)
		{
			Print("Cheating detected!");
			m_bOpponentCheating = true;
			SCR_ScenarioFrameworkSystem.GetCallQueuePausable().Remove(TryMakeMove);
		}
		
		m_bMyTurn = true;
		SCR_ScenarioFrameworkSystem.GetCallQueuePausable().CallLater(TryMakeMove, m_iDelay * 1000, false);
	}
	
	private void TryMakeMove()
	{
		if (m_Board.m_iWinner != 0 || m_bOpponentCheating)
			return;
		
		bool isPlayerPeeking = GetGame().GetPlayerController().GetPlayerCamera().IsSphereVisible(GetOwner().GetOrigin(), 1);
		if (isPlayerPeeking)
		{
			Print("Player is peeking. Delaying move.");
			SCR_ScenarioFrameworkSystem.GetCallQueuePausable().CallLater(TryMakeMove, m_iDelay * 1000, false);
			return;
		}
		
		m_bMyTurn = false;
		int x = 0;
		AlphaBetaMinMax(m_Board.m_aState, m_iMinMaxDepth, -INFINITY, INFINITY, m_iAIPlayer, x);
		m_Board.AskPlaceStone(x, m_iAIPlayer);
	}
	
	private float AlphaBetaMinMax(int state[7][6], int depth, float alpha, float beta, int player, out int bestColumn)
	{
		int winner = GetWinner(state);
		if (winner != 0)
		{
			if (winner == m_iAIPlayer)
				return 1000;
			else
				return -1000;
		}
		if (depth == 0)
		{
        	return UtilityScore(state, m_iAIPlayer);
		}
		
		int throwAway;
	    if (player == m_iAIPlayer)
		{
	        float max_eval = -INFINITY;
	        for (int x; x < 7; x++)
			{
				if (!CanPlace(state, x))
					continue;
				
				int newState[7][6];
				GetResultingState(state, x, player, newState);
	            float eval = AlphaBetaMinMax(newState, depth - 1, alpha, beta, m_iOpponentPlayer, throwAway);
	            if (max_eval < eval)
				{
					max_eval = eval;
					bestColumn = x;
				}
	            alpha = Math.Max(alpha, eval);
	            if (beta <= alpha)
	                break;  // Beta cut-off
			}
	        return max_eval;
		}
	    else
		{
	        float min_eval = INFINITY;
	        for (int x; x < 7; x++)
			{
				if (!CanPlace(state, x))
					continue;
				
				int newState[7][6];
				GetResultingState(state, x, player, newState);
	            float eval = AlphaBetaMinMax(newState, depth - 1, alpha, beta, m_iAIPlayer, throwAway);
	            if (min_eval > eval)
				{
					min_eval = eval;
					bestColumn = x;
				}
	            beta = Math.Min(beta, eval);
	            if (beta <= alpha)
	                break;  // Alpha cut-off
			} 
	        return min_eval;
		}
	}
	
	private int GetWinner(int state[7][6])
	{
		for (int x; x < 7; x++)
		{
			for (int y; y < 6; y++)
			{
				int p = state[x][y];
				if (p == 0)
					continue;
				// Vertical (counting down)
				if (y > 2 && state[x][y-1] == p && state[x][y-2] == p && state[x][y-3] == p)
					return p;
				// Horizontal (counting left)
				if (x > 2 && state[x-1][y] == p && state[x-2][y] == p && state[x-3][y] == p)
					return p;
				// Diagonal (counting left and down)
				if (x > 2 && y > 2 && state[x-1][y-1] == p && state[x-2][y-2] == p && state[x-3][y-3] == p)
					return p;
				// Diagonal (counting left and up)
				if (x > 2 && y < 4 && state[x-1][y+1] == p && state[x-2][y+2] == p && state[x-3][y+3] == p)
					return p;
			}
		}
		return 0;
	}
	
	private int UtilityScore(int state[7][6], int player)
	{
		int score = 0;
		for (int x; x < 7; x++)
		{
			for (int y; y < 6; y++)
			{
				int p = state[x][y];
				if (p != player)
					continue;
				// Vertical (counting down)
				if (y > 2)
				{
					if (state[x][y-1] == player)
						score += 1;
					if (state[x][y-2] == player)
						score += 1;
					if (state[x][y-3] == player)
						score += 1;
				}
				// Horizontal (counting left)
				if (x > 2)
				{
					if (state[x-1][y] == player)
						score += 1;
					if (state[x-2][y] == player)
						score += 1;
					if (state[x-3][y] == player)
						score += 1;
				}
				// Diagonal (counting left and down)
				if (x > 2 && y > 2)
				{
					if (state[x-1][y-1] == player)
						score += 1;
					if (state[x-2][y-2] == player)
						score += 1;
					if (state[x-3][y-3] == player)
						score += 1;
				}
				// Diagonal (counting left and up)
				if (x > 2 && y < 4)
				{
					if (state[x-1][y+1] == player)
						score += 1;
					if (state[x-2][y+2] == player)
						score += 1;
					if (state[x-3][y+3] == player)
						score += 1;
				}
			}
		}
		return score;
	}
	
	private bool CanPlace(int state[7][6], int x)
	{
		return state[x][5] == 0;
	}
	
	private void GetResultingState(int state[7][6], int x, int player, out int newState[7][6])
	{
		for (int xx; xx < 7; xx++)
		{
			for (int y; y < 6; y++)
			{
				newState[xx][y] = state[xx][y];
			}
		}
		
		for (int y; y < 6; y++)
		{
			if (newState[x][y] == 0)
			{
				newState[x][y] = player;
				return;
			}
		}
	}
}
